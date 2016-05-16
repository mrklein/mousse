// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "near_wall_fields.hpp"
#include "word_re_list.hpp"
#include "find_cell_particle.hpp"
#include "mapped_patch_base.hpp"
#include "obj_stream.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(nearWallFields, 0);

}


// Private Member Functions 
void mousse::nearWallFields::calcAddressing()
{
  const fvMesh& mesh = refCast<const fvMesh>(obr_);
  // Count number of faces
  label nPatchFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter) {
    label patchI = iter.key();
    nPatchFaces += mesh.boundary()[patchI].size();
  }
  // Global indexing
  globalIndex globalWalls{nPatchFaces};
  if (debug) {
    Info << "nearWallFields::calcAddressing() :"
      << " nPatchFaces:" << globalWalls.size() << endl;
  }
  // Construct cloud
  Cloud<findCellParticle> cloud{mesh, IDLList<findCellParticle>{}};
  // Add particles to track to sample locations
  nPatchFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchSet_, iter) {
    label patchI = iter.key();
    const fvPatch& patch = mesh.boundary()[patchI];
    vectorField nf{patch.nf()};
    vectorField faceCellCentres{patch.patch().faceCellCentres()};
    FOR_ALL(patch, patchFaceI) {
      label meshFaceI = patch.start()+patchFaceI;
      // Find starting point on face (since faceCentre might not
      // be on face-diagonal decomposition)
      pointIndexHit startInfo
      {
        mappedPatchBase::facePoint
        (
          mesh,
          meshFaceI,
          polyMesh::FACE_DIAG_TRIS
        )
      };
      point start;
      if (startInfo.hit()) {
        start = startInfo.hitPoint();
      } else {
        // Fallback: start tracking from neighbouring cell centre
        start = faceCellCentres[patchFaceI];
      }
      const point end = start-distance_*nf[patchFaceI];
      // Find tet for starting location
      label cellI = -1;
      label tetFaceI = -1;
      label tetPtI = -1;
      mesh.findCellFacePt(start, cellI, tetFaceI, tetPtI);
      // Add to cloud. Add originating face as passive data
      cloud.addParticle
      (
        new findCellParticle
        {
          mesh,
          start,
          cellI,
          tetFaceI,
          tetPtI,
          end,
          globalWalls.toGlobal(nPatchFaces)    // passive data
        }
      );
      nPatchFaces++;
    }
  }
  if (debug) {
    // Dump particles
    OBJstream str
    {
      mesh.time().path()
      /"wantedTracks_" + mesh.time().timeName() + ".obj"
    };
    Info << "nearWallFields::calcAddressing() :"
      << "Dumping tracks to " << str.name() << endl;
    FOR_ALL_CONST_ITER(Cloud<findCellParticle>, cloud, iter) {
      const findCellParticle& tp = iter();
      str.write(linePointRef(tp.position(), tp.end()));
    }
  }
  // Per cell: empty or global wall index and end location
  cellToWalls_.setSize(mesh.nCells());
  cellToSamples_.setSize(mesh.nCells());
  // Database to pass into findCellParticle::move
  findCellParticle::trackingData td{cloud, cellToWalls_, cellToSamples_};
  // Track all particles to their end position.
  scalar maxTrackLen = 2.0*mesh.bounds().mag();
  //Debug: collect start points
  pointField start;
  if (debug) {
    start.setSize(nPatchFaces);
    nPatchFaces = 0;
    FOR_ALL_CONST_ITER(Cloud<findCellParticle>, cloud, iter) {
      const findCellParticle& tp = iter();
      start[nPatchFaces++] = tp.position();
    }
  }
  cloud.move(td, maxTrackLen);
  // Rework cell-to-globalpatchface into a map
  List<Map<label>> compactMap;
  getPatchDataMapPtr_.reset
  (
    new mapDistribute
    {
      globalWalls,
      cellToWalls_,
      compactMap
      }
  );
  // Debug: dump resulting tracks
  if (debug) {
    getPatchDataMapPtr_().distribute(start);
    {
      OBJstream str
      {
        mesh.time().path()
        /"obtainedTracks_" + mesh.time().timeName() + ".obj"
      };
      Info << "nearWallFields::calcAddressing() :"
        << "Dumping obtained to " << str.name() << endl;
      FOR_ALL(cellToWalls_, cellI) {
        const List<point>& ends = cellToSamples_[cellI];
        const labelList& cData = cellToWalls_[cellI];
        FOR_ALL(cData, i) {
          str.write(linePointRef(ends[i], start[cData[i]]));
        }
      }
    }
  }
}


// Constructors 
mousse::nearWallFields::nearWallFields
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool /*loadFromFiles*/
)
:
  name_{name},
  obr_{obr},
  active_{true},
  fieldSet_{}
{
  // Check if the available mesh is an fvMesh otherise deactivate
  if (isA<fvMesh>(obr_)) {
    read(dict);
  } else {
    active_ = false;
    WARNING_IN
    (
      "nearWallFields::nearWallFields"
      "("
      "  const word&, "
      "  const objectRegistry&, "
      "  const dictionary&, "
      "  const bool"
      ")"
    )
    << "No fvMesh available, deactivating " << name_
    << endl;
  }
}


// Destructor 
mousse::nearWallFields::~nearWallFields()
{
  if (debug) {
    Info << "nearWallFields::~nearWallFields()" << endl;
  }
}


// Member Functions 
void mousse::nearWallFields::read(const dictionary& dict)
{
  if (debug) {
    Info << "nearWallFields::read(const dictionary&)" << endl;
  }
  if (active_) {
    const fvMesh& mesh = refCast<const fvMesh>(obr_);
    dict.lookup("fields") >> fieldSet_;
    patchSet_ =
      mesh.boundaryMesh().patchSet(wordReList(dict.lookup("patches")));
    distance_ = readScalar(dict.lookup("distance"));
    // Clear out any previously loaded fields
    vsf_.clear();
    vvf_.clear();
    vSpheretf_.clear();
    vSymmtf_.clear();
    vtf_.clear();
    fieldMap_.clear();
    reverseFieldMap_.clear();
    // Generate fields with mappedField boundary condition
    // Convert field to map
    fieldMap_.resize(2*fieldSet_.size());
    reverseFieldMap_.resize(2*fieldSet_.size());
    FOR_ALL(fieldSet_, setI) {
      const word& fldName = fieldSet_[setI].first();
      const word& sampleFldName = fieldSet_[setI].second();
      fieldMap_.insert(fldName, sampleFldName);
      reverseFieldMap_.insert(sampleFldName, fldName);
    }
    Info << type() << " " << name_ << ": Sampling " << fieldMap_.size()
      << " fields" << endl;
    // Do analysis
    calcAddressing();
  }
}


void mousse::nearWallFields::execute()
{
  if (debug) {
    Info << "nearWallFields:execute()" << endl;
  }
  if (!active_)
    return;
  if (fieldMap_.size()
      && vsf_.empty()
      && vvf_.empty()
      && vSpheretf_.empty()
      && vSymmtf_.empty()
      && vtf_.empty()) {
    Info << type() << " " << name_ << ": Creating " << fieldMap_.size()
      << " fields" << endl;
    createFields(vsf_);
    createFields(vvf_);
    createFields(vSpheretf_);
    createFields(vSymmtf_);
    createFields(vtf_);
    Info << endl;
  }
  Info << type() << " " << name_ << " output:" << nl;
  Info << "    Sampling fields to " << obr_.time().timeName() << endl;
  sampleFields(vsf_);
  sampleFields(vvf_);
  sampleFields(vSpheretf_);
  sampleFields(vSymmtf_);
  sampleFields(vtf_);
}


void mousse::nearWallFields::end()
{
  if (debug) {
    Info << "nearWallFields:end()" << endl;
  }
  if (active_) {
    execute();
  }
}


void mousse::nearWallFields::timeSet()
{
  // Do nothing
}


void mousse::nearWallFields::write()
{
  if (debug) {
    Info<< "nearWallFields:write()" << endl;
  }
  if (!active_)
    return;
  Info << "    Writing sampled fields to " << obr_.time().timeName() << endl;
  FOR_ALL(vsf_, i) {
    vsf_[i].write();
  }
  FOR_ALL(vvf_, i) {
    vvf_[i].write();
  }
  FOR_ALL(vSpheretf_, i) {
    vSpheretf_[i].write();
  }
  FOR_ALL(vSymmtf_, i) {
    vSymmtf_[i].write();
  }
  FOR_ALL(vtf_, i) {
    vtf_[i].write();
  }
  Info << endl;
}

