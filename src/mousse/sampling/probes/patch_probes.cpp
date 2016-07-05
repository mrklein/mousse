// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_probes.hpp"
#include "vol_fields.hpp"
#include "iomanip.hpp"
// For 'nearInfo' helper class only
#include "mapped_patch_base.hpp"
#include "tree_bound_box.hpp"
#include "tree_data_face.hpp"
#include "empty_poly_patch.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(patchProbes, 0);

}


// Private Member Functions 
void mousse::patchProbes::findElements(const fvMesh& mesh)
{
  (void)mesh.tetBasePtIs();
  const polyBoundaryMesh& bm = mesh.boundaryMesh();
  label patchI = bm.findPatchID(patchName_);
  if (patchI == -1) {
    FATAL_ERROR_IN
    (
      " mousse::patchProbes::findElements(const fvMesh&)"
    )
    << " Unknown patch name "
    << patchName_ << endl
    << exit(FatalError);
  }
  // All the info for nearest. Construct to miss
  List<mappedPatchBase::nearInfo> nearest{this->size()};
  const polyPatch& pp = bm[patchI];
  if (pp.size() > 0) {
    labelList bndFaces{pp.size()};
    FOR_ALL(bndFaces, i) {
      bndFaces[i] =  pp.start() + i;
    }
    treeBoundBox overallBb{pp.points()};
    Random rndGen{123456};
    overallBb = overallBb.extend(rndGen, 1e-4);
    overallBb.min() -= point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    overallBb.max() += point(ROOTVSMALL, ROOTVSMALL, ROOTVSMALL);
    const indexedOctree<treeDataFace> boundaryTree
    {
      {
        false,                      // do not cache bb
        mesh,
        bndFaces                    // patch faces only
      },
      overallBb,                      // overall search domain
      8,                              // maxLevel
      10,                             // leafsize
      3.0                             // duplicity
    };
    FOR_ALL(probeLocations(), probeI) {
      const point sample = probeLocations()[probeI];
      scalar span = boundaryTree.bb().mag();
      pointIndexHit info = boundaryTree.findNearest(sample, mousse::sqr(span));
      if (!info.hit()) {
        info = boundaryTree.findNearest(sample, mousse::sqr(GREAT));
      }
      label faceI = boundaryTree.shapes().faceLabels()[info.index()];
      const label patchi = bm.whichPatch(faceI);
      if (isA<emptyPolyPatch>(bm[patchi])) {
        WARNING_IN
        (
          " mousse::patchProbes::findElements(const fvMesh&)"
        )
        << " The sample point: " << sample
        << " belongs to " << patchi
        << " which is an empty patch. This is not permitted. "
        << " This sample will not be included "
        << endl;
      } else {
        const point& fc = mesh.faceCentres()[faceI];
        mappedPatchBase::nearInfo sampleInfo;
        sampleInfo.first() = pointIndexHit(true, fc, faceI);
        sampleInfo.second().first() = magSqr(fc-sample);
        sampleInfo.second().second() = Pstream::myProcNo();
        nearest[probeI]= sampleInfo;
      }
    }
  }
  // Find nearest.
  Pstream::listCombineGather(nearest, mappedPatchBase::nearestEqOp());
  Pstream::listCombineScatter(nearest);
  if (debug) {
    Info << "patchProbes::findElements" << " : " << endl;
    FOR_ALL(nearest, sampleI) {
      label procI = nearest[sampleI].second().second();
      label localI = nearest[sampleI].first().index();
      Info << "    " << sampleI << " coord:"<< operator[](sampleI)
        << " found on processor:" << procI
        << " in local cell/face:" << localI
        << " with fc:" << nearest[sampleI].first().rawPoint() << endl;
    }
  }
  // Extract any local faces to sample
  elementList_.setSize(nearest.size(), -1);
  FOR_ALL(nearest, sampleI) {
    if (nearest[sampleI].second().second() == Pstream::myProcNo()) {
      // Store the face to sample
      elementList_[sampleI] = nearest[sampleI].first().index();
    }
  }
}


// Constructors 
mousse::patchProbes::patchProbes
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  probes{name, obr, dict, loadFromFiles}
{
  // When constructing probes above it will have called the
  // probes::findElements (since the virtual mechanism not yet operating).
  // Not easy to workaround (apart from feeding through flag into constructor)
  // so clear out any cells found for now.
  elementList_.clear();
  faceList_.clear();
  read(dict);
}


// Destructor 
mousse::patchProbes::~patchProbes()
{}


void mousse::patchProbes::write()
{
  if (!this->size() || !prepare())
    return;
  sampleAndWrite(scalarFields_);
  sampleAndWrite(vectorFields_);
  sampleAndWrite(sphericalTensorFields_);
  sampleAndWrite(symmTensorFields_);
  sampleAndWrite(tensorFields_);
  sampleAndWriteSurfaceFields(surfaceScalarFields_);
  sampleAndWriteSurfaceFields(surfaceVectorFields_);
  sampleAndWriteSurfaceFields(surfaceSphericalTensorFields_);
  sampleAndWriteSurfaceFields(surfaceSymmTensorFields_);
  sampleAndWriteSurfaceFields(surfaceTensorFields_);
}


void mousse::patchProbes::read(const dictionary& dict)
{
  dict.lookup("patchName") >> patchName_;
  probes::read(dict);
}

