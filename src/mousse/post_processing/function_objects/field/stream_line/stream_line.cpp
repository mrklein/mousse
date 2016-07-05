// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pstream.hpp"
#include "function_object_list.hpp"
#include "stream_line.hpp"
#include "fv_mesh.hpp"
#include "stream_line_particle_cloud.hpp"
#include "_read_fields.hpp"
#include "mesh_search.hpp"
#include "sampled_set.hpp"
#include "global_index.hpp"
#include "map_distribute.hpp"
#include "interpolation_cell_point.hpp"
#include "patch_tools.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(streamLine, 0);

}


// Private Member Functions 
mousse::autoPtr<mousse::indirectPrimitivePatch>
mousse::streamLine::wallPatch() const
{
  const fvMesh& mesh = dynamic_cast<const fvMesh&>(obr_);
  const polyBoundaryMesh& patches = mesh.boundaryMesh();
  label nFaces = 0;
  FOR_ALL(patches, patchI) {
    //if (!polyPatch::constraintType(patches[patchI].type()))
    if (isA<wallPolyPatch>(patches[patchI])) {
      nFaces += patches[patchI].size();
    }
  }
  labelList addressing{nFaces};
  nFaces = 0;
  FOR_ALL(patches, patchI) {
    if (!isA<wallPolyPatch>(patches[patchI]))
      continue;
    const polyPatch& pp = patches[patchI];
    FOR_ALL(pp, i) {
      addressing[nFaces++] = pp.start()+i;
    }
  }
  return
    autoPtr<indirectPrimitivePatch>
    {
      new indirectPrimitivePatch
      {
        IndirectList<face>{mesh.faces(), addressing},
        mesh.points()
      }
    };
}


void mousse::streamLine::track()
{
  const Time& runTime = obr_.time();
  const fvMesh& mesh = dynamic_cast<const fvMesh&>(obr_);
  IDLList<streamLineParticle> initialParticles;
  streamLineParticleCloud particles{mesh, cloudName_, initialParticles};
  const sampledSet& seedPoints = sampledSetPtr_();
  FOR_ALL(seedPoints, i) {
    particles.addParticle
    (
      new streamLineParticle
      {
        mesh,
        seedPoints[i],
        seedPoints.cells()[i],
        lifeTime_               // lifetime
      }
    );
  }
  label nSeeds = returnReduce(particles.size(), sumOp<label>());
  Info << "    seeded " << nSeeds << " particles" << endl;
  // Read or lookup fields
  PtrList<volScalarField> vsFlds;
  PtrList<interpolation<scalar>> vsInterp;
  PtrList<volVectorField> vvFlds;
  PtrList<interpolation<vector>> vvInterp;
  label UIndex = -1;
  if (loadFromFiles_) {
    IOobjectList allObjects{mesh, runTime.timeName()};
    IOobjectList objects{2*fields_.size()};
    FOR_ALL(fields_, i) {
      objects.add(*allObjects[fields_[i]]);
    }
    ReadFields(mesh, objects, vsFlds);
    vsInterp.setSize(vsFlds.size());
    FOR_ALL(vsFlds, i) {
      vsInterp.set
      (
        i,
        interpolation<scalar>::New
        (
          interpolationScheme_,
          vsFlds[i]
        )
      );
    }
    ReadFields(mesh, objects, vvFlds);
    vvInterp.setSize(vvFlds.size());
    FOR_ALL(vvFlds, i) {
      vvInterp.set
      (
        i,
        interpolation<vector>::New
        (
          interpolationScheme_,
          vvFlds[i]
        )
      );
    }
  } else {
    label nScalar = 0;
    label nVector = 0;
    FOR_ALL(fields_, i) {
      if (mesh.foundObject<volScalarField>(fields_[i])) {
        nScalar++;
      } else if (mesh.foundObject<volVectorField>(fields_[i])) {
        nVector++;
      } else {
        FATAL_ERROR_IN("streamLine::track()")
          << "Cannot find field " << fields_[i] << nl
          << "Valid scalar fields are:"
          << mesh.names(volScalarField::typeName) << nl
          << "Valid vector fields are:"
          << mesh.names(volVectorField::typeName)
          << exit(FatalError);
      }
    }
    vsInterp.setSize(nScalar);
    nScalar = 0;
    vvInterp.setSize(nVector);
    nVector = 0;
    FOR_ALL(fields_, i) {
      if (mesh.foundObject<volScalarField>(fields_[i])) {
        const volScalarField& f =
          mesh.lookupObject<volScalarField>(fields_[i]);
        vsInterp.set
        (
          nScalar++,
          interpolation<scalar>::New
          (
            interpolationScheme_,
            f
          )
        );
      } else if (mesh.foundObject<volVectorField>(fields_[i])) {
        const volVectorField& f =
          mesh.lookupObject<volVectorField>(fields_[i]);
        if (f.name() == UName_) {
          UIndex = nVector;
        }
        vvInterp.set
        (
          nVector++,
          interpolation<vector>::New
          (
            interpolationScheme_,
            f
          )
        );
      }
    }
  }
  // Store the names
  scalarNames_.setSize(vsInterp.size());
  FOR_ALL(vsInterp, i) {
    scalarNames_[i] = vsInterp[i].psi().name();
  }
  vectorNames_.setSize(vvInterp.size());
  FOR_ALL(vvInterp, i) {
    vectorNames_[i] = vvInterp[i].psi().name();
  }
  // Check that we know the index of U in the interpolators.
  if (UIndex == -1) {
    FATAL_ERROR_IN("streamLine::track()")
      << "Cannot find field to move particles with : " << UName_ << nl
      << "This field has to be present in the sampled fields " << fields_
      << " and in the objectRegistry."
      << exit(FatalError);
  }
  // Sampled data
  // ~~~~~~~~~~~~
  // Size to maximum expected sizes.
  allTracks_.clear();
  allTracks_.setCapacity(nSeeds);
  allScalars_.setSize(vsInterp.size());
  FOR_ALL(allScalars_, i) {
    allScalars_[i].clear();
    allScalars_[i].setCapacity(nSeeds);
  }
  allVectors_.setSize(vvInterp.size());
  FOR_ALL(allVectors_, i) {
    allVectors_[i].clear();
    allVectors_[i].setCapacity(nSeeds);
  }
  // additional particle info
  streamLineParticle::trackingData td
  {
    particles,
    vsInterp,
    vvInterp,
    UIndex,         // index of U in vvInterp
    trackForward_,  // track in +u direction?
    nSubCycle_,     // automatic track control:step through cells in steps?
    trackLength_,   // fixed track length
    allTracks_,
    allScalars_,
    allVectors_
  };
  // Set very large dt. Note: cannot use GREAT since 1/GREAT is SMALL
  // which is a trigger value for the tracking...
  const scalar trackTime = mousse::sqrt(GREAT);
  // Track
  particles.move(td, trackTime);
}


// Constructors 
mousse::streamLine::streamLine
(
  const word& name,
  const objectRegistry& obr,
  const dictionary& dict,
  const bool loadFromFiles
)
:
  dict_{dict},
  name_{name},
  obr_{obr},
  loadFromFiles_{loadFromFiles},
  active_{true},
  nSubCycle_{0}
{
  // Only active if a fvMesh is available
  if (isA<fvMesh>(obr_)) {
    read(dict_);
  } else {
    active_ = false;
    WARNING_IN
    (
      "streamLine::streamLine\n"
      "(\n"
      "  const word&,\n"
      "  const objectRegistry&,\n"
      "  const dictionary&,\n"
      "  const bool\n"
      ")"
    )
    << "No fvMesh available, deactivating."
    << nl << endl;
  }
}


// Destructor 
mousse::streamLine::~streamLine()
{}


// Member Functions 
void mousse::streamLine::read(const dictionary& dict)
{
  if (!active_)
    return;
  Info << type() << " " << name_ << ":" << nl;
  //dict_ = dict;
  dict.lookup("fields") >> fields_;
  if (dict.found("UName")) {
    dict.lookup("UName") >> UName_;
  } else {
    UName_ = "U";
    if (dict.found("U")) {
      IO_WARNING_IN("streamLine::read(const dictionary&)", dict)
        << "Using deprecated entry \"U\"."
        << " Please use \"UName\" instead."
        << endl;
      dict.lookup("U") >> UName_;
    }
  }
  if (findIndex(fields_, UName_) == -1) {
    FATAL_IO_ERROR_IN("streamLine::read(const dictionary&)", dict)
      << "Velocity field for tracking " << UName_
      << " should be present in the list of fields " << fields_
      << exit(FatalIOError);
  }
  dict.lookup("trackForward") >> trackForward_;
  dict.lookup("lifeTime") >> lifeTime_;
  if (lifeTime_ < 1) {
    FATAL_ERROR_IN(":streamLine::read(const dictionary&)")
      << "Illegal value " << lifeTime_ << " for lifeTime"
      << exit(FatalError);
  }
  bool subCycling = dict.found("nSubCycle");
  bool fixedLength = dict.found("trackLength");
  if (subCycling && fixedLength) {
    FATAL_IO_ERROR_IN("streamLine::read(const dictionary&)", dict)
      << "Cannot both specify automatic time stepping (through '"
      << "nSubCycle' specification) and fixed track length (through '"
      << "trackLength')"
      << exit(FatalIOError);
  }
  nSubCycle_ = 1;
  if (dict.readIfPresent("nSubCycle", nSubCycle_)) {
    trackLength_ = VGREAT;
    if (nSubCycle_ < 1) {
      nSubCycle_ = 1;
    }
    Info << "    automatic track length specified through"
      << " number of sub cycles : " << nSubCycle_ << nl << endl;
  } else {
    dict.lookup("trackLength") >> trackLength_;
    Info << "    fixed track length specified : "
      << trackLength_ << nl << endl;
  }
  interpolationScheme_ =
    dict.lookupOrDefault
    (
      "interpolationScheme",
      interpolationCellPoint<scalar>::typeName
    );
  cloudName_ = dict.lookupOrDefault<word>("cloudName", "streamLine");
  dict.lookup("seedSampleSet") >> seedSet_;
  const fvMesh& mesh = dynamic_cast<const fvMesh&>(obr_);
  meshSearchPtr_.reset(new meshSearch{mesh});
  const dictionary& coeffsDict = dict.subDict(seedSet_ + "Coeffs");
  sampledSetPtr_ =
    sampledSet::New
    (
      seedSet_,
      mesh,
      meshSearchPtr_(),
      coeffsDict
    );
  coeffsDict.lookup("axis") >> sampledSetAxis_;
  scalarFormatterPtr_ = writer<scalar>::New(dict.lookup("setFormat"));
  vectorFormatterPtr_ = writer<vector>::New(dict.lookup("setFormat"));
}


void mousse::streamLine::execute()
{}


void mousse::streamLine::end()
{}


void mousse::streamLine::timeSet()
{}


void mousse::streamLine::write()
{
  if (!active_)
    return;
  Info << type() << " " << name_ << " output:" << nl;
  const Time& runTime = obr_.time();
  const fvMesh& mesh = dynamic_cast<const fvMesh&>(obr_);
  // Do all injection and tracking
  track();
  if (Pstream::parRun()) {
    // Append slave tracks to master ones
    globalIndex globalTrackIDs{allTracks_.size()};
    // Construct a distribution map to pull all to the master.
    labelListList sendMap{Pstream::nProcs()};
    labelListList recvMap{Pstream::nProcs()};
    if (Pstream::master()) {
      // Master: receive all. My own first, then consecutive
      // processors.
      label trackI = 0;
      FOR_ALL(recvMap, procI) {
        labelList& fromProc = recvMap[procI];
        fromProc.setSize(globalTrackIDs.localSize(procI));
        FOR_ALL(fromProc, i) {
          fromProc[i] = trackI++;
        }
      }
    }
    labelList& toMaster = sendMap[0];
    toMaster.setSize(globalTrackIDs.localSize());
    FOR_ALL(toMaster, i) {
      toMaster[i] = i;
    }
    const mapDistribute distMap
    {
      globalTrackIDs.size(),
      sendMap.xfer(),
      recvMap.xfer()
    };
    // Distribute the track positions. Note: use scheduled comms
    // to prevent buffering.
    mapDistribute::distribute
      (
        Pstream::scheduled,
        distMap.schedule(),
        distMap.constructSize(),
        distMap.subMap(),
        distMap.constructMap(),
        allTracks_
      );
    // Distribute the scalars
    FOR_ALL(allScalars_, scalarI) {
      mapDistribute::distribute
        (
          Pstream::scheduled,
          distMap.schedule(),
          distMap.constructSize(),
          distMap.subMap(),
          distMap.constructMap(),
          allScalars_[scalarI]
        );
    }
    // Distribute the vectors
    FOR_ALL(allVectors_, vectorI) {
      mapDistribute::distribute
        (
          Pstream::scheduled,
          distMap.schedule(),
          distMap.constructSize(),
          distMap.subMap(),
          distMap.constructMap(),
          allVectors_[vectorI]
        );
    }
  }
  label n = 0;
  FOR_ALL(allTracks_, trackI) {
    n += allTracks_[trackI].size();
  }
  Info << "    Tracks:" << allTracks_.size() << nl
    << "    Total samples:" << n
    << endl;
  // Massage into form suitable for writers
  if (Pstream::master() && allTracks_.size()) {
    // Make output directory
    fileName vtkPath
    {
      Pstream::parRun()
      ? runTime.path()/".."/"postProcessing"/"sets"/name()
      : runTime.path()/"postProcessing"/"sets"/name()
    };
    if (mesh.name() != fvMesh::defaultRegion) {
      vtkPath = vtkPath/mesh.name();
    }
    vtkPath = vtkPath/mesh.time().timeName();
    mkDir(vtkPath);
    // Convert track positions
    PtrList<coordSet> tracks{allTracks_.size()};
    FOR_ALL(allTracks_, trackI) {
      tracks.set
      (
        trackI,
        new coordSet
        {
          "track" + mousse::name(trackI),
          sampledSetAxis_                 //"xyz"
        }
      );
      tracks[trackI].transfer(allTracks_[trackI]);
    }
    // Convert scalar values
    if (allScalars_.size() > 0) {
      List<List<scalarField>> scalarValues{allScalars_.size()};
      FOR_ALL(allScalars_, scalarI) {
        DynamicList<scalarList>& allTrackVals = allScalars_[scalarI];
        scalarValues[scalarI].setSize(allTrackVals.size());
        FOR_ALL(allTrackVals, trackI) {
          scalarList& trackVals = allTrackVals[trackI];
          scalarValues[scalarI][trackI].transfer(trackVals);
        }
      }
      fileName vtkFile
      {
        vtkPath/scalarFormatterPtr_().getFileName(tracks[0], scalarNames_)
      };
      Info << "    Writing data to " << vtkFile.path() << endl;
      scalarFormatterPtr_().write
        (
          true,           // writeTracks
          tracks,
          scalarNames_,
          scalarValues,
          OFstream{vtkFile}()
        );
    }
    // Convert vector values
    if (allVectors_.size() > 0) {
      List<List<vectorField>> vectorValues{allVectors_.size()};
      FOR_ALL(allVectors_, vectorI) {
        DynamicList<vectorList>& allTrackVals = allVectors_[vectorI];
        vectorValues[vectorI].setSize(allTrackVals.size());
        FOR_ALL(allTrackVals, trackI) {
          vectorList& trackVals = allTrackVals[trackI];
          vectorValues[vectorI][trackI].transfer(trackVals);
        }
      }
      fileName vtkFile
      {
        vtkPath/vectorFormatterPtr_().getFileName(tracks[0], vectorNames_)
      };
      vectorFormatterPtr_().write
      (
        true,           // writeTracks
        tracks,
        vectorNames_,
        vectorValues,
        OFstream{vtkFile}()
      );
    }
  }
}


void mousse::streamLine::updateMesh(const mapPolyMesh&)
{
  read(dict_);
}


void mousse::streamLine::movePoints(const polyMesh&)
{
  // Moving mesh affects the search tree
  read(dict_);
}

