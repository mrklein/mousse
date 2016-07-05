// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mapped_patch_base.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "list_list_ops.hpp"
#include "mesh_search_mesh_object.hpp"
#include "mesh_tools.hpp"
#include "ofstream.hpp"
#include "random.hpp"
#include "tree_data_face.hpp"
#include "tree_data_point.hpp"
#include "indexed_octree.hpp"
#include "poly_mesh.hpp"
#include "poly_patch.hpp"
#include "time.hpp"
#include "map_distribute.hpp"
#include "sub_field.hpp"
#include "tri_point_ref.hpp"
#include "sync_tools.hpp"
#include "tree_data_cell.hpp"
#include "dynamic_field.hpp"
#include "pstream_reduce_ops.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(mappedPatchBase, 0);
template<>
const char* mousse::NamedEnum
<
  mousse::mappedPatchBase::sampleMode,
  6
>::names[] =
{
  "nearestCell",
  "nearestPatchFace",
  "nearestPatchFaceAMI",
  "nearestPatchPoint",
  "nearestFace",
  "nearestOnlyCell"
};
template<>
const char* mousse::NamedEnum
<
  mousse::mappedPatchBase::offsetMode,
  3
>::names[] =
{
  "uniform",
  "nonuniform",
  "normal"
};

}

const mousse::NamedEnum<mousse::mappedPatchBase::sampleMode, 6>
  mousse::mappedPatchBase::sampleModeNames_;

const mousse::NamedEnum<mousse::mappedPatchBase::offsetMode, 3>
  mousse::mappedPatchBase::offsetModeNames_;


// Private Member Functions
mousse::tmp<mousse::pointField> mousse::mappedPatchBase::facePoints
(
  const polyPatch& pp
) const
{
  const polyMesh& mesh = pp.boundaryMesh().mesh();
  // Force construction of min-tet decomp
  (void)mesh.tetBasePtIs();
  // Initialise to face-centre
  tmp<pointField> tfacePoints{new pointField{patch_.size()}};
  pointField& facePoints = tfacePoints();
  FOR_ALL(pp, faceI) {
    facePoints[faceI] = facePoint
    (
      mesh,
      pp.start()+faceI,
      polyMesh::FACE_DIAG_TRIS
    ).rawPoint();
  }
  return tfacePoints;
}


void mousse::mappedPatchBase::collectSamples
(
  const pointField& facePoints,
  pointField& samples,
  labelList& patchFaceProcs,
  labelList& patchFaces,
  pointField& patchFc
) const
{
  // Collect all sample points and the faces they come from.
  {
    List<pointField> globalFc{Pstream::nProcs()};
    globalFc[Pstream::myProcNo()] = facePoints;
    Pstream::gatherList(globalFc);
    Pstream::scatterList(globalFc);
    // Rework into straight list
    patchFc = ListListOps::combine<pointField>
    (
      globalFc,
      accessOp<pointField>()
    );
  }

  {
    List<pointField> globalSamples{Pstream::nProcs()};
    globalSamples[Pstream::myProcNo()] = samplePoints(facePoints);
    Pstream::gatherList(globalSamples);
    Pstream::scatterList(globalSamples);
    // Rework into straight list
    samples = ListListOps::combine<pointField>
    (
      globalSamples,
      accessOp<pointField>()
    );
  }

  {
    labelListList globalFaces{Pstream::nProcs()};
    globalFaces[Pstream::myProcNo()] = identity(patch_.size());
    // Distribute to all processors
    Pstream::gatherList(globalFaces);
    Pstream::scatterList(globalFaces);
    patchFaces = ListListOps::combine<labelList>
    (
      globalFaces,
      accessOp<labelList>()
    );
  }

  {
    labelList nPerProc{Pstream::nProcs()};
    nPerProc[Pstream::myProcNo()] = patch_.size();
    Pstream::gatherList(nPerProc);
    Pstream::scatterList(nPerProc);
    patchFaceProcs.setSize(patchFaces.size());
    label sampleI = 0;
    FOR_ALL(nPerProc, procI) {
      for (label i = 0; i < nPerProc[procI]; i++) {
        patchFaceProcs[sampleI++] = procI;
      }
    }
  }
}


// Find the processor/cell containing the samples. Does not account
// for samples being found in two processors.
void mousse::mappedPatchBase::findSamples
(
  const sampleMode mode,
  const pointField& samples,
  labelList& sampleProcs,
  labelList& sampleIndices,
  pointField& sampleLocations
) const
{
  // Lookup the correct region
  const polyMesh& mesh = sampleMesh();
  // All the info for nearest. Construct to miss
  List<nearInfo> nearest{samples.size()};
  switch (mode) {
    case NEARESTCELL:
    {
      if (samplePatch_.size() && samplePatch_ != "none") {
        FATAL_ERROR_IN
        (
          "mappedPatchBase::findSamples(const pointField&,"
          " labelList&, labelList&, pointField&) const"
        )
        << "No need to supply a patch name when in "
        << sampleModeNames_[mode] << " mode." << exit(FatalError);
      }
      //- Note: face-diagonal decomposition
      const indexedOctree<mousse::treeDataCell>& tree = mesh.cellTree();
      FOR_ALL(samples, sampleI) {
        const point& sample = samples[sampleI];
        label cellI = tree.findInside(sample);
        if (cellI == -1) {
          nearest[sampleI].second().first() = mousse::sqr(GREAT);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        } else {
          const point& cc = mesh.cellCentres()[cellI];
          nearest[sampleI].first() = pointIndexHit
          (
            true,
            cc,
            cellI
          );
          nearest[sampleI].second().first() = magSqr(cc-sample);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        }
      }
      break;
    }
    case NEARESTONLYCELL:
    {
      if (samplePatch_.size() && samplePatch_ != "none") {
        FATAL_ERROR_IN
        (
          "mappedPatchBase::findSamples(const pointField&,"
          " labelList&, labelList&, pointField&) const"
        )
        << "No need to supply a patch name when in "
        << sampleModeNames_[mode] << " mode." << exit(FatalError);
      }
      //- Note: face-diagonal decomposition
      const indexedOctree<mousse::treeDataCell>& tree = mesh.cellTree();
      FOR_ALL(samples, sampleI) {
        const point& sample = samples[sampleI];
        nearest[sampleI].first() = tree.findNearest(sample, sqr(GREAT));
        nearest[sampleI].second().first() = magSqr
        (
          nearest[sampleI].first().hitPoint()
         -sample
        );
        nearest[sampleI].second().second() = Pstream::myProcNo();
      }
      break;
    }
    case NEARESTPATCHFACE:
    {
      Random rndGen{123456};
      const polyPatch& pp = samplePolyPatch();
      if (pp.empty()) {
        FOR_ALL(samples, sampleI) {
          nearest[sampleI].second().first() = mousse::sqr(GREAT);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        }
      } else {
        // patch faces
        const labelList patchFaces(identity(pp.size()) + pp.start());
        treeBoundBox patchBb
        {
          treeBoundBox(pp.points(), pp.meshPoints()).extend
          (
            rndGen,
            1e-4
          )
        };
        patchBb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
        patchBb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
        indexedOctree<treeDataFace> boundaryTree
        {
          treeDataFace    // all information needed to search faces
          {
            false,      // do not cache bb
            mesh,
            patchFaces  // boundary faces only
          },
          patchBb,        // overall search domain
          8,              // maxLevel
          10,             // leafsize
          3.0             // duplicity
        };
        FOR_ALL(samples, sampleI) {
          const point& sample = samples[sampleI];
          pointIndexHit& nearInfo = nearest[sampleI].first();
          nearInfo = boundaryTree.findNearest
          (
            sample,
            magSqr(patchBb.span())
          );
          if (!nearInfo.hit()) {
            nearest[sampleI].second().first() = mousse::sqr(GREAT);
            nearest[sampleI].second().second() = Pstream::myProcNo();
          } else {
            point fc(pp[nearInfo.index()].centre(pp.points()));
            nearInfo.setPoint(fc);
            nearest[sampleI].second().first() = magSqr(fc-sample);
            nearest[sampleI].second().second() = Pstream::myProcNo();
          }
        }
      }
      break;
    }
    case NEARESTPATCHPOINT:
    {
      Random rndGen{123456};
      const polyPatch& pp = samplePolyPatch();
      if (pp.empty()) {
        FOR_ALL(samples, sampleI) {
          nearest[sampleI].second().first() = mousse::sqr(GREAT);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        }
      } else {
        // patch (local) points
        treeBoundBox patchBb
        {
          treeBoundBox(pp.points(), pp.meshPoints()).extend
          (
            rndGen,
            1e-4
          )
        };
        patchBb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
        patchBb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
        indexedOctree<treeDataPoint> boundaryTree
        {
          treeDataPoint   // all information needed to search faces
          {
            mesh.points(),
            pp.meshPoints() // selection of points to search on
          },
          patchBb,        // overall search domain
          8,              // maxLevel
          10,             // leafsize
          3.0             // duplicity
        };
        FOR_ALL(samples, sampleI) {
          const point& sample = samples[sampleI];
          pointIndexHit& nearInfo = nearest[sampleI].first();
          nearInfo = boundaryTree.findNearest
          (
            sample,
            magSqr(patchBb.span())
          );
          if (!nearInfo.hit()) {
            nearest[sampleI].second().first() = mousse::sqr(GREAT);
            nearest[sampleI].second().second() = Pstream::myProcNo();
          } else {
            const point& pt = nearInfo.hitPoint();
            nearest[sampleI].second().first() = magSqr(pt-sample);
            nearest[sampleI].second().second() = Pstream::myProcNo();
          }
        }
      }
      break;
    }
    case NEARESTFACE:
    {
      if (samplePatch().size() && samplePatch() != "none") {
        FATAL_ERROR_IN
        (
          "mappedPatchBase::findSamples(const pointField&,"
          " labelList&, labelList&, pointField&) const"
        )
        << "No need to supply a patch name when in "
        << sampleModeNames_[mode] << " mode." << exit(FatalError);
      }
      //- Note: face-diagonal decomposition
      const meshSearchMeshObject& meshSearchEngine =
        meshSearchMeshObject::New(mesh);
      FOR_ALL(samples, sampleI) {
        const point& sample = samples[sampleI];
        label faceI = meshSearchEngine.findNearestFace(sample);
        if (faceI == -1) {
          nearest[sampleI].second().first() = mousse::sqr(GREAT);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        } else {
          const point& fc = mesh.faceCentres()[faceI];
          nearest[sampleI].first() = pointIndexHit
          (
            true,
            fc,
            faceI
          );
          nearest[sampleI].second().first() = magSqr(fc-sample);
          nearest[sampleI].second().second() = Pstream::myProcNo();
        }
      }
      break;
    }
    case NEARESTPATCHFACEAMI:
    {
      // nothing to do here
      return;
    }
    default:
    {
      FATAL_ERROR_IN("mappedPatchBase::findSamples(..)")
        << "problem." << abort(FatalError);
    }
  }
  // Find nearest. Combine on master.
  Pstream::listCombineGather(nearest, nearestEqOp());
  Pstream::listCombineScatter(nearest);
  if (debug) {
    Info << "mappedPatchBase::findSamples on mesh " << sampleRegion()
      << " : " << endl;
    FOR_ALL(nearest, sampleI) {
      label procI = nearest[sampleI].second().second();
      label localI = nearest[sampleI].first().index();
      Info << "    " << sampleI << " coord:"<< samples[sampleI]
        << " found on processor:" << procI
        << " in local cell/face/point:" << localI
        << " with location:" << nearest[sampleI].first().rawPoint()
        << endl;
    }
  }
  // Convert back into proc+local index
  sampleProcs.setSize(samples.size());
  sampleIndices.setSize(samples.size());
  sampleLocations.setSize(samples.size());
  FOR_ALL(nearest, sampleI) {
    if (!nearest[sampleI].first().hit()) {
      sampleProcs[sampleI] = -1;
      sampleIndices[sampleI] = -1;
      sampleLocations[sampleI] = vector::max;
    } else {
      sampleProcs[sampleI] = nearest[sampleI].second().second();
      sampleIndices[sampleI] = nearest[sampleI].first().index();
      sampleLocations[sampleI] = nearest[sampleI].first().hitPoint();
    }
  }
}


void mousse::mappedPatchBase::calcMapping() const
{
  static bool hasWarned = false;
  if (mapPtr_.valid()) {
    FATAL_ERROR_IN("mappedPatchBase::calcMapping() const")
      << "Mapping already calculated" << exit(FatalError);
  }
  // Get points on face (since cannot use face-centres - might be off
  // face-diagonal decomposed tets.
  tmp<pointField> patchPoints{facePoints(patch_)};
  // Get offsetted points
  const pointField offsettedPoints{samplePoints(patchPoints())};
  // Do a sanity check - am I sampling my own patch?
  // This only makes sense for a non-zero offset.
  bool sampleMyself =
  (
    mode_ == NEARESTPATCHFACE
    && sampleRegion() == patch_.boundaryMesh().mesh().name()
    && samplePatch() == patch_.name()
  );
  // Check offset
  vectorField d{offsettedPoints-patchPoints()};
  bool coincident = (gAverage(mag(d)) <= ROOTVSMALL);
  if (sampleMyself && coincident) {
    WARNING_IN
    (
      "mappedPatchBase::mappedPatchBase\n"
      "(\n"
      "    const polyPatch& pp,\n"
      "    const word& sampleRegion,\n"
      "    const sampleMode mode,\n"
      "    const word& samplePatch,\n"
      "    const vector& offset\n"
      ")\n"
    )
    << "Invalid offset " << d << endl
    << "Offset is the vector added to the patch face centres to"
    << " find the patch face supplying the data." << endl
    << "Setting it to " << d
    << " on the same patch, on the same region"
    << " will find the faces themselves which does not make sense"
    << " for anything but testing." << endl
    << "patch_:" << patch_.name() << endl
    << "sampleRegion_:" << sampleRegion() << endl
    << "mode_:" << sampleModeNames_[mode_] << endl
    << "samplePatch_:" << samplePatch() << endl
    << "offsetMode_:" << offsetModeNames_[offsetMode_] << endl;
  }
  // Get global list of all samples and the processor and face they come from.
  pointField samples;
  labelList patchFaceProcs;
  labelList patchFaces;
  pointField patchFc;
  collectSamples
  (
    patchPoints,
    samples,
    patchFaceProcs,
    patchFaces,
    patchFc
  );
  // Find processor and cell/face samples are in and actual location.
  labelList sampleProcs;
  labelList sampleIndices;
  pointField sampleLocations;
  findSamples(mode_, samples, sampleProcs, sampleIndices, sampleLocations);
  // Check for samples that were not found. This will only happen for
  // NEARESTCELL since finds cell containing a location
  if (mode_ == NEARESTCELL) {
    label nNotFound = 0;
    FOR_ALL(sampleProcs, sampleI) {
      if (sampleProcs[sampleI] == -1) {
        nNotFound++;
      }
    }
    reduce(nNotFound, sumOp<label>());
    if (nNotFound > 0) {
      if (!hasWarned) {
        WARNING_IN
        (
          "mappedPatchBase::mappedPatchBase\n"
          "(\n"
          "    const polyPatch& pp,\n"
          "    const word& sampleRegion,\n"
          "    const sampleMode mode,\n"
          "    const word& samplePatch,\n"
          "    const vector& offset\n"
          ")\n"
        )
        << "Did not find " << nNotFound
        << " out of " << sampleProcs.size() << " total samples."
        << " Sampling these on owner cell centre instead." << endl
        << "On patch " << patch_.name()
        << " on region " << sampleRegion()
        << " in mode " << sampleModeNames_[mode_] << endl
        << "with offset mode " << offsetModeNames_[offsetMode_]
        << ". Suppressing further warnings from " << type() << endl;
        hasWarned = true;
      }
      // Collect the samples that cannot be found
      DynamicList<label> subMap;
      DynamicField<point> subSamples;
      FOR_ALL(sampleProcs, sampleI) {
        if (sampleProcs[sampleI] == -1) {
          subMap.append(sampleI);
          subSamples.append(samples[sampleI]);
        }
      }
      // And re-search for pure nearest (should not fail)
      labelList subSampleProcs;
      labelList subSampleIndices;
      pointField subSampleLocations;
      findSamples
      (
        NEARESTONLYCELL,
        subSamples,
        subSampleProcs,
        subSampleIndices,
        subSampleLocations
      );
      // Insert
      UIndirectList<label>(sampleProcs, subMap) = subSampleProcs;
      UIndirectList<label>(sampleIndices, subMap) = subSampleIndices;
      UIndirectList<point>(sampleLocations, subMap) = subSampleLocations;
    }
  }
  if (debug && Pstream::master()) {
    OFstream str
    {
      patch_.boundaryMesh().mesh().time().path()/patch_.name() + "_mapped.obj"
    };
    Pout << "Dumping mapping as lines from patch faceCentres to"
      << " sampled cell/faceCentres/points to file " << str.name()
      << endl;
    label vertI = 0;
    FOR_ALL(patchFc, i) {
      meshTools::writeOBJ(str, patchFc[i]);
      vertI++;
      meshTools::writeOBJ(str, sampleLocations[i]);
      vertI++;
      str << "l " << vertI-1 << ' ' << vertI << nl;
    }
  }
  // Determine schedule.
  mapPtr_.reset(new mapDistribute{sampleProcs, patchFaceProcs});
  // Rework the schedule from indices into samples to cell data to send,
  // face data to receive.
  labelListList& subMap = mapPtr_().subMap();
  labelListList& constructMap = mapPtr_().constructMap();
  FOR_ALL(subMap, procI) {
    subMap[procI] = UIndirectList<label>
    (
      sampleIndices,
      subMap[procI]
    );
    constructMap[procI] = UIndirectList<label>
    (
      patchFaces,
      constructMap[procI]
    );
  }
  // Redo constructSize
  mapPtr_().constructSize() = patch_.size();
  if (debug) {
    // Check that all elements get a value.
    PackedBoolList used{patch_.size()};
    FOR_ALL(constructMap, procI) {
      const labelList& map = constructMap[procI];
      FOR_ALL(map, i) {
        label faceI = map[i];
        if (used[faceI] == 0) {
          used[faceI] = 1;
        } else {
          FATAL_ERROR_IN("mappedPatchBase::calcMapping() const")
            << "On patch " << patch_.name()
            << " patchface " << faceI
            << " is assigned to more than once."
            << abort(FatalError);
        }
      }
    }
    FOR_ALL(used, faceI) {
      if (used[faceI] == 0) {
        FATAL_ERROR_IN("mappedPatchBase::calcMapping() const")
          << "On patch " << patch_.name()
          << " patchface " << faceI
          << " is never assigned to."
          << abort(FatalError);
      }
    }
  }
}


const mousse::autoPtr<mousse::searchableSurface>& mousse::mappedPatchBase::surfPtr()
const
{
  const word surfType(surfDict_.lookupOrDefault<word>("type", "none"));
  if (!surfPtr_.valid() && surfType != "none") {
    word surfName(surfDict_.lookupOrDefault("name", patch_.name()));
    const polyMesh& mesh = patch_.boundaryMesh().mesh();
    surfPtr_ =
      searchableSurface::New
      (
        surfType,
        {
          surfName,
          mesh.time().constant(),
          "triSurface",
          mesh,
          IOobject::MUST_READ,
          IOobject::NO_WRITE
        },
        surfDict_
      );
  }
  return surfPtr_;
}


void mousse::mappedPatchBase::calcAMI() const
{
  if (AMIPtr_.valid()) {
    FATAL_ERROR_IN("mappedPatchBase::calcAMI() const")
      << "AMI already calculated" << exit(FatalError);
  }
  AMIPtr_.clear();
  if (debug) {
    const polyPatch& nbr = samplePolyPatch();
    pointField nbrPoints(nbr.localPoints());
    OFstream os{patch_.name() + "_neighbourPatch-org.obj"};
    meshTools::writeOBJ(os, samplePolyPatch().localFaces(), nbrPoints);
    // transform neighbour patch to local system
    primitivePatch nbrPatch0
    {
      SubList<face>
      {
        nbr.localFaces(),
        nbr.size()
      },
      nbrPoints
    };
    OFstream osN{patch_.name() + "_neighbourPatch-trans.obj"};
    meshTools::writeOBJ(osN, nbrPatch0, nbrPoints);
    OFstream osO{patch_.name() + "_ownerPatch.obj"};
    meshTools::writeOBJ(osO, patch_.localFaces(), patch_.localPoints());
  }
  // Construct/apply AMI interpolation to determine addressing and weights
  AMIPtr_.reset
  (
    new AMIPatchToPatchInterpolation
    {
      patch_,
      samplePolyPatch(), // nbrPatch0,
      surfPtr(),
      faceAreaIntersect::tmMesh,
      true,
      AMIPatchToPatchInterpolation::imFaceAreaWeight,
      -1,
      AMIReverse_
    }
  );
}


// Hack to read old (List-based) format. See Field.C. The difference
// is only that in case of falling back to old format it expects a non-uniform
// list instead of a single vector.
mousse::tmp<mousse::pointField> mousse::mappedPatchBase::readListOrField
(
  const word& keyword,
  const dictionary& dict,
  const label size
)
{
  tmp<pointField> tfld{new pointField{}};
  pointField& fld = tfld();
  if (size) {
    ITstream& is = dict.lookup(keyword);
    // Read first token
    token firstToken{is};
    if (firstToken.isWord()) {
      if (firstToken.wordToken() == "uniform") {
        fld.setSize(size);
        fld = pTraits<vector>(is);
      } else if (firstToken.wordToken() == "nonuniform") {
        is >> static_cast<List<vector>&>(fld);
        if (fld.size() != size) {
          FATAL_IO_ERROR_IN
          (
            "mappedPatchBase::readListOrField"
            "(const word& keyword, const dictionary&, const label)",
            dict
          )
          << "size " << fld.size()
          << " is not equal to the given value of " << size
          << exit(FatalIOError);
        }
      } else {
        FATAL_IO_ERROR_IN
        (
          "mappedPatchBase::readListOrField"
          "(const word& keyword, const dictionary&, const label)",
          dict
        )
        << "expected keyword 'uniform' or 'nonuniform', found "
        << firstToken.wordToken()
        << exit(FatalIOError);
      }
    } else {
      if (is.version() == 2.0) {
        IO_WARNING_IN
        (
          "mappedPatchBase::readListOrField"
          "(const word& keyword, const dictionary&, const label)",
          dict
        )
        << "expected keyword 'uniform' or 'nonuniform', "
           "assuming List format for backwards compatibility."
           "Foam version 2.0." << endl;
        is.putBack(firstToken);
        is >> static_cast<List<vector>&>(fld);
      }
    }
  }
  return tfld;
}


// Constructors
mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp
)
:
  patch_{pp},
  sampleRegion_{patch_.boundaryMesh().mesh().name()},
  mode_{NEARESTPATCHFACE},
  samplePatch_{""},
  coupleGroup_(),
  offsetMode_{UNIFORM},
  offset_{vector::zero},
  offsets_{pp.size(), offset_},
  distance_{0},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{false},
  surfPtr_{NULL},
  surfDict_{fileName("surface")}
{}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const word& sampleRegion,
  const sampleMode mode,
  const word& samplePatch,
  const vectorField& offsets
)
:
  patch_{pp},
  sampleRegion_{sampleRegion},
  mode_{mode},
  samplePatch_{samplePatch},
  coupleGroup_(),
  offsetMode_{NONUNIFORM},
  offset_{vector::zero},
  offsets_{offsets},
  distance_{0},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{false},
  surfPtr_{NULL},
  surfDict_{fileName("surface")}
{}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const word& sampleRegion,
  const sampleMode mode,
  const word& samplePatch,
  const vector& offset
)
:
  patch_{pp},
  sampleRegion_{sampleRegion},
  mode_{mode},
  samplePatch_{samplePatch},
  coupleGroup_(),
  offsetMode_{UNIFORM},
  offset_{offset},
  offsets_{0},
  distance_{0},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{false},
  surfPtr_{NULL},
  surfDict_{fileName("surface")}
{}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const word& sampleRegion,
  const sampleMode mode,
  const word& samplePatch,
  const scalar distance
)
:
  patch_{pp},
  sampleRegion_{sampleRegion},
  mode_{mode},
  samplePatch_{samplePatch},
  coupleGroup_(),
  offsetMode_{NORMAL},
  offset_{vector::zero},
  offsets_{0},
  distance_{distance},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{false},
  surfPtr_{NULL},
  surfDict_{fileName("surface")}
{}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const dictionary& dict
)
:
  patch_{pp},
  sampleRegion_{dict.lookupOrDefault<word>("sampleRegion", "")},
  mode_{sampleModeNames_.read(dict.lookup("sampleMode"))},
  samplePatch_{dict.lookupOrDefault<word>("samplePatch", "")},
  coupleGroup_{dict},
  offsetMode_{UNIFORM},
  offset_{vector::zero},
  offsets_{0},
  distance_{0.0},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{dict.lookupOrDefault<bool>("flipNormals", false)},
  surfPtr_{NULL},
  surfDict_{dict.subOrEmptyDict("surface")}
{
  if (!coupleGroup_.valid()) {
    if (sampleRegion_.empty()) {
      // If no coupleGroup and no sampleRegion assume local region
      sampleRegion_ = patch_.boundaryMesh().mesh().name();
      sameRegion_ = true;
    }
  }
  if (dict.found("offsetMode")) {
    offsetMode_ = offsetModeNames_.read(dict.lookup("offsetMode"));
    switch(offsetMode_) {
      case UNIFORM:
      {
        offset_ = point(dict.lookup("offset"));
      }
      break;
      case NONUNIFORM:
      {
        //offsets_ = pointField(dict.lookup("offsets"));
        offsets_ = readListOrField("offsets", dict, patch_.size());
      }
      break;
      case NORMAL:
      {
        distance_ = readScalar(dict.lookup("distance"));
      }
      break;
    }
  } else if (dict.found("offset")) {
    offsetMode_ = UNIFORM;
    offset_ = point(dict.lookup("offset"));
  } else if (dict.found("offsets")) {
    offsetMode_ = NONUNIFORM;
    //offsets_ = pointField(dict.lookup("offsets"));
    offsets_ = readListOrField("offsets", dict, patch_.size());
  } else if (mode_ != NEARESTPATCHFACE && mode_ != NEARESTPATCHFACEAMI) {
    FATAL_IO_ERROR_IN
    (
      "mappedPatchBase::mappedPatchBase\n"
      "(\n"
      "  const polyPatch&,\n"
      "  const dictionary&\n"
      ")\n",
      dict
    )
    << "Please supply the offsetMode as one of "
    << NamedEnum<offsetMode, 3>::words()
    << exit(FatalIOError);
  }
}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const sampleMode mode,
  const dictionary& dict
)
:
  patch_{pp},
  sampleRegion_{dict.lookupOrDefault<word>("sampleRegion", "")},
  mode_{mode},
  samplePatch_{dict.lookupOrDefault<word>("samplePatch", "")},
  coupleGroup_{dict}, //dict.lookupOrDefault<word>("coupleGroup", "")},
  offsetMode_{UNIFORM},
  offset_{vector::zero},
  offsets_{0},
  distance_{0.0},
  sameRegion_{sampleRegion_ == patch_.boundaryMesh().mesh().name()},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{dict.lookupOrDefault<bool>("flipNormals", false)},
  surfPtr_{NULL},
  surfDict_{dict.subOrEmptyDict("surface")}
{
  if (mode != NEARESTPATCHFACE && mode != NEARESTPATCHFACEAMI) {
    FATAL_IO_ERROR_IN
    (
      "mappedPatchBase::mappedPatchBase\n"
      "(\n"
      "    const polyPatch&,\n"
      "    const sampleMode,\n"
      "    const dictionary&\n"
      ")\n",
      dict
    )
    << "Construct from sampleMode and dictionary only applicable for "
    << " collocated patches in modes "
    << sampleModeNames_[NEARESTPATCHFACE] << ','
    << sampleModeNames_[NEARESTPATCHFACEAMI]
    << exit(FatalIOError);
  }
  if (!coupleGroup_.valid()) {
    if (sampleRegion_.empty()) {
      // If no coupleGroup and no sampleRegion assume local region
      sampleRegion_ = patch_.boundaryMesh().mesh().name();
      sameRegion_ = true;
    }
  }
}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const mappedPatchBase& mpb
)
:
  patch_{pp},
  sampleRegion_{mpb.sampleRegion_},
  mode_{mpb.mode_},
  samplePatch_{mpb.samplePatch_},
  coupleGroup_{mpb.coupleGroup_},
  offsetMode_{mpb.offsetMode_},
  offset_{mpb.offset_},
  offsets_{mpb.offsets_},
  distance_{mpb.distance_},
  sameRegion_{mpb.sameRegion_},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{mpb.AMIReverse_},
  surfPtr_{NULL},
  surfDict_{mpb.surfDict_}
{}


mousse::mappedPatchBase::mappedPatchBase
(
  const polyPatch& pp,
  const mappedPatchBase& mpb,
  const labelUList& mapAddressing
)
:
  patch_{pp},
  sampleRegion_{mpb.sampleRegion_},
  mode_{mpb.mode_},
  samplePatch_{mpb.samplePatch_},
  coupleGroup_{mpb.coupleGroup_},
  offsetMode_{mpb.offsetMode_},
  offset_{mpb.offset_},
  offsets_
  {
    offsetMode_ == NONUNIFORM
    ? vectorField(mpb.offsets_, mapAddressing)
    : vectorField(0)
  },
  distance_{mpb.distance_},
  sameRegion_{mpb.sameRegion_},
  mapPtr_{NULL},
  AMIPtr_{NULL},
  AMIReverse_{mpb.AMIReverse_},
  surfPtr_{NULL},
  surfDict_{mpb.surfDict_}
{}


// Destructor
mousse::mappedPatchBase::~mappedPatchBase()
{
  clearOut();
}


void mousse::mappedPatchBase::clearOut()
{
  mapPtr_.clear();
  AMIPtr_.clear();
  surfPtr_.clear();
}


// Member Functions
const mousse::polyMesh& mousse::mappedPatchBase::sampleMesh() const
{
  return patch_.boundaryMesh().mesh().time().lookupObject<polyMesh>
  (
    sampleRegion()
  );
}


const mousse::polyPatch& mousse::mappedPatchBase::samplePolyPatch() const
{
  const polyMesh& nbrMesh = sampleMesh();
  const label patchI = nbrMesh.boundaryMesh().findPatchID(samplePatch());
  if (patchI == -1) {
    FATAL_ERROR_IN("mappedPatchBase::samplePolyPatch()")
      << "Cannot find patch " << samplePatch()
      << " in region " << sampleRegion_ << endl
      << "Valid patches are " << nbrMesh.boundaryMesh().names()
      << exit(FatalError);
  }
  return nbrMesh.boundaryMesh()[patchI];
}


mousse::tmp<mousse::pointField> mousse::mappedPatchBase::samplePoints
(
  const pointField& fc
) const
{
  tmp<pointField> tfld{new pointField{fc}};
  pointField& fld = tfld();
  switch (offsetMode_) {
    case UNIFORM:
    {
      fld += offset_;
      break;
    }
    case NONUNIFORM:
    {
      fld += offsets_;
      break;
    }
    case NORMAL:
    {
      // Get outwards pointing normal
      vectorField n(patch_.faceAreas());
      n /= mag(n);
      fld += distance_*n;
      break;
    }
  }
  return tfld;
}


mousse::tmp<mousse::pointField> mousse::mappedPatchBase::samplePoints() const
{
  return samplePoints(facePoints(patch_));
}


mousse::pointIndexHit mousse::mappedPatchBase::facePoint
(
  const polyMesh& mesh,
  const label faceI,
  const polyMesh::cellDecomposition decompMode
)
{
  const point& fc = mesh.faceCentres()[faceI];
  switch (decompMode) {
    case polyMesh::FACE_PLANES:
    case polyMesh::FACE_CENTRE_TRIS:
    {
      // For both decompositions the face centre is guaranteed to be
      // on the face
      return {true, fc, faceI};
    }
    break;
    case polyMesh::FACE_DIAG_TRIS:
    case polyMesh::CELL_TETS:
    {
      // Find the intersection of a ray from face centre to cell centre
      // Find intersection of (face-centre-decomposition) centre to
      // cell-centre with face-diagonal-decomposition triangles.
      const pointField& p = mesh.points();
      const face& f = mesh.faces()[faceI];
      if (f.size() <= 3)
      {
        // Return centre of triangle.
        return {true, fc, 0};
      }
      label cellI = mesh.faceOwner()[faceI];
      const point& cc = mesh.cellCentres()[cellI];
      vector d = fc-cc;
      const label fp0 = mesh.tetBasePtIs()[faceI];
      const point& basePoint = p[f[fp0]];
      label fp = f.fcIndex(fp0);
      for (label i = 2; i < f.size(); i++)
      {
        const point& thisPoint = p[f[fp]];
        label nextFp = f.fcIndex(fp);
        const point& nextPoint = p[f[nextFp]];
        const triPointRef tri(basePoint, thisPoint, nextPoint);
        pointHit hitInfo = tri.intersection
        (
          cc,
          d,
          intersection::HALF_RAY
        );
        if (hitInfo.hit() && hitInfo.distance() > 0) {
          return pointIndexHit(true, hitInfo.hitPoint(), i-2);
        }
        fp = nextFp;
      }
      // Fall-back
      return {false, fc, -1};
    }
    break;
    default:
    {
      FATAL_ERROR_IN("mappedPatchBase::facePoint()")
        << "problem" << abort(FatalError);
      return pointIndexHit();
    }
  }
}


void mousse::mappedPatchBase::write(Ostream& os) const
{
  os.writeKeyword("sampleMode") << sampleModeNames_[mode_]
    << token::END_STATEMENT << nl;
  if (!sampleRegion_.empty()) {
    os.writeKeyword("sampleRegion") << sampleRegion_
      << token::END_STATEMENT << nl;
  }
  if (!samplePatch_.empty()) {
    os.writeKeyword("samplePatch") << samplePatch_
      << token::END_STATEMENT << nl;
  }
  coupleGroup_.write(os);
  if (offsetMode_ == UNIFORM
      && offset_ == vector::zero
      && (mode_ == NEARESTPATCHFACE || mode_ == NEARESTPATCHFACEAMI)) {
    // Collocated mode. No need to write offset data
  } else {
    os.writeKeyword("offsetMode") << offsetModeNames_[offsetMode_]
      << token::END_STATEMENT << nl;
    switch (offsetMode_) {
      case UNIFORM:
      {
        os.writeKeyword("offset") << offset_ << token::END_STATEMENT
          << nl;
        break;
      }
      case NONUNIFORM:
      {
        offsets_.writeEntry("offsets", os);
        break;
      }
      case NORMAL:
      {
        os.writeKeyword("distance") << distance_ << token::END_STATEMENT
          << nl;
        break;
      }
    }
    if (mode_ == NEARESTPATCHFACEAMI) {
      if (AMIReverse_) {
        os.writeKeyword("flipNormals") << AMIReverse_
          << token::END_STATEMENT << nl;
      }
      if (!surfDict_.empty()) {
        os.writeKeyword(surfDict_.dictName());
        os  << surfDict_;
      }
    }
  }
}

