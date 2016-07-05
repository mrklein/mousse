// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_patch_agglomeration.hpp"
#include "mesh_tools.hpp"
#include "unit_conversion.hpp"


// Private Member Functions 
void mousse::pairPatchAgglomeration::compactLevels(const label nCreatedLevels)
{
  nFaces_.setSize(nCreatedLevels);
  restrictAddressing_.setSize(nCreatedLevels);
  patchLevels_.setSize(nCreatedLevels);
}


bool mousse::pairPatchAgglomeration::continueAgglomerating
(
  const label nCoarseFaces
)
{
  // Check the need for further agglomeration on all processors
  label localnCoarseFaces = nCoarseFaces;
  bool contAgg = localnCoarseFaces >= nFacesInCoarsestLevel_;
  return contAgg;
}


void mousse::pairPatchAgglomeration::setBasedEdgeWeights()
{
  const bPatch& coarsePatch = patchLevels_[0];
  FOR_ALL(coarsePatch.edges(), i) {
    if (!coarsePatch.isInternalEdge(i))
      continue;
    scalar edgeLength = coarsePatch.edges()[i].mag(coarsePatch.localPoints());
    const labelList& eFaces = coarsePatch.edgeFaces()[i];
    if (eFaces.size() == 2) {
      scalar cosI =
        coarsePatch.faceNormals()[eFaces[0]]
        & coarsePatch.faceNormals()[eFaces[1]];
      const edge edgeCommon = edge(eFaces[0], eFaces[1]);
      if (facePairWeight_.found(edgeCommon)) {
        facePairWeight_[edgeCommon] += edgeLength;
      } else {
        facePairWeight_.insert(edgeCommon, edgeLength);
      }
      if (cosI < mousse::cos(degToRad(featureAngle_))) {
        facePairWeight_[edgeCommon] = -1.0;
      }
    } else {
      FOR_ALL(eFaces, j) {
        for (label k = j+1; k<eFaces.size(); k++) {
          facePairWeight_.insert
          (
            edge(eFaces[j], eFaces[k]),
            -1.0
          );
        }
      }
    }
  }
}


void mousse::pairPatchAgglomeration::setEdgeWeights
(
  const label fineLevelIndex
)
{
  const bPatch& coarsePatch = patchLevels_[fineLevelIndex];
  const labelList& fineToCoarse = restrictAddressing_[fineLevelIndex];
  const label nCoarseI =  max(fineToCoarse) + 1;
  labelListList coarseToFine{invertOneToMany(nCoarseI, fineToCoarse)};
  HashSet<edge, Hash<edge>> fineFeaturedFaces{coarsePatch.nEdges()/10};
  // Map fine faces with featured edge into coarse faces
  FOR_ALL_CONST_ITER(EdgeMap<scalar>, facePairWeight_, iter) {
    if (iter() == -1.0) {
      const edge e = iter.key();
      const edge edgeFeatured{fineToCoarse[e[0]], fineToCoarse[e[1]]};
      fineFeaturedFaces.insert(edgeFeatured);
    }
  }
  // Clean old weitghs
  facePairWeight_.clear();
  facePairWeight_.resize(coarsePatch.nEdges());
  FOR_ALL(coarsePatch.edges(), i) {
    if (!coarsePatch.isInternalEdge(i))
      continue;
    scalar edgeLength = coarsePatch.edges()[i].mag(coarsePatch.localPoints());
    const labelList& eFaces = coarsePatch.edgeFaces()[i];
    if (eFaces.size() == 2) {
      const edge edgeCommon = edge(eFaces[0], eFaces[1]);
      if (facePairWeight_.found(edgeCommon)) {
        facePairWeight_[edgeCommon] += edgeLength;
      } else {
        facePairWeight_.insert(edgeCommon, edgeLength);
      }
      // If the fine 'pair' faces was featured edge so it is
      // the coarse 'pair'
      if (fineFeaturedFaces.found(edgeCommon)) {
        facePairWeight_[edgeCommon] = -1.0;
      }
    } else {
      // Set edge as barrier by setting weight to -1
      FOR_ALL(eFaces, j) {
        for (label k = j+1; k<eFaces.size(); k++) {
          facePairWeight_.insert
            (
              edge(eFaces[j], eFaces[k]),
              -1.0
            );
        }
      }
    }
  }
}


// Constructors 
mousse::pairPatchAgglomeration::pairPatchAgglomeration
(
  const polyPatch& patch,
  const dictionary& controlDict,
  const bool /*additionalWeights*/
)
:
  mergeLevels_
  {
    controlDict.lookupOrDefault<label>("mergeLevels", 2)
  },
  maxLevels_{50},
  nFacesInCoarsestLevel_
  {
    readLabel(controlDict.lookup("nFacesInCoarsestLevel"))
  },
  featureAngle_
  {
    controlDict.lookupOrDefault<scalar>("featureAngle", 0)
  },
  nFaces_{maxLevels_},
  restrictAddressing_{maxLevels_},
  restrictTopBottomAddressing_{identity(patch.size())},
  patchLevels_{maxLevels_},
  facePairWeight_{patch.size()}
{
  // Set base fine patch
  patchLevels_.set
  (
    0,
    new bPatch
    {
      patch.localFaces(),
      patch.localPoints()
    }
  );
  // Set number of faces for the base patch
  nFaces_[0] = patch.size();
  // Set edge weights for level 0
  setBasedEdgeWeights();
}


// Destructor 
mousse::pairPatchAgglomeration::~pairPatchAgglomeration()
{}


// Member Functions 
const mousse::bPatch& mousse::pairPatchAgglomeration::patchLevel
(
  const label i
) const
{
  return patchLevels_[i];
}


void mousse::pairPatchAgglomeration::mapBaseToTopAgglom
(
  const label fineLevelIndex
)
{
  const labelList& fineToCoarse = restrictAddressing_[fineLevelIndex];
  FOR_ALL(restrictTopBottomAddressing_, i) {
    restrictTopBottomAddressing_[i] =
      fineToCoarse[restrictTopBottomAddressing_[i]];
  }
}


bool mousse::pairPatchAgglomeration::agglomeratePatch
(
  const bPatch& patch,
  const labelList& fineToCoarse,
  const label fineLevelIndex
)
{
  if (min(fineToCoarse) == -1) {
    FATAL_ERROR_IN
    (
      "pairPatchAgglomeration::agglomeratePatch"
      "("
        "const bPatch&, "
        "const labelList&, "
        "const label"
      ")"
    )
    << "min(fineToCoarse) == -1" << exit(FatalError);
  }
  if (fineToCoarse.size() == 0) {
    return true;
  }
  if (fineToCoarse.size() != patch.size()) {
    FATAL_ERROR_IN
    (
      "pairPatchAgglomeration::agglomeratePatch"
      "("
        "const bPatch&, "
        "const labelList&, "
        "const label"
      ")"
    )
    << "restrict map does not correspond to fine level. " << endl
    << " Sizes: restrictMap: " << fineToCoarse.size()
    << " nEqns: " << patch.size()
    << abort(FatalError);
  }
  const label nCoarseI =  max(fineToCoarse) + 1;
  List<face> patchFaces{nCoarseI};
  // Patch faces per agglomeration
  labelListList coarseToFine{invertOneToMany(nCoarseI, fineToCoarse)};
  for (label coarseI = 0; coarseI < nCoarseI; coarseI++) {
    const labelList& fineFaces = coarseToFine[coarseI];
    // Construct single face
    indirectPrimitivePatch upp
    {
      IndirectList<face>{patch, fineFaces},
      patch.points()
    };
    if (upp.edgeLoops().size() != 1) {
      if (fineFaces.size() == 2) {
        const edge e(fineFaces[0], fineFaces[1]);
        facePairWeight_[e] = -1.0;
      } else if (fineFaces.size() == 3) {
        const edge e(fineFaces[0], fineFaces[1]);
        const edge e1(fineFaces[0], fineFaces[2]);
        const edge e2(fineFaces[2], fineFaces[1]);
        facePairWeight_[e] = -1.0;
        facePairWeight_[e1] = -1.0;
        facePairWeight_[e2] = -1.0;
      }
      return false;
    }
    patchFaces[coarseI] =
      face
      {
        renumber
        (
          upp.meshPoints(),
          upp.edgeLoops()[0]
        )
      };
  }
  patchLevels_.set
  (
    fineLevelIndex,
    new bPatch
    {
      SubList<face>{patchFaces, nCoarseI, 0},
      patch.points()
    }
  );
  return true;
}


void mousse::pairPatchAgglomeration::agglomerate()
{
  label nPairLevels = 0;
  label nCreatedLevels = 1; // 0 level is the base patch
  label nCoarseFaces = 0;
  label nCoarseFacesOld = 0;
  while (nCreatedLevels < maxLevels_) {
    const bPatch& patch = patchLevels_[nCreatedLevels - 1];
    tmp<labelField> finalAgglomPtr{new labelField{patch.size()}};
    bool agglomOK = false;
    do {
      label nCoarseFacesPrev = nCoarseFaces;
      finalAgglomPtr =
        agglomerateOneLevel
        (
          nCoarseFaces,
          patch
        );
      if (nCoarseFaces > 0 && nCoarseFaces != nCoarseFacesPrev) {
        if ((agglomOK = agglomeratePatch
             (
               patch,
               finalAgglomPtr,
               nCreatedLevels
             ))) {
          restrictAddressing_.set(nCreatedLevels, finalAgglomPtr);
          mapBaseToTopAgglom(nCreatedLevels);
          setEdgeWeights(nCreatedLevels);
          if (nPairLevels % mergeLevels_) {
            combineLevels(nCreatedLevels);
          } else {
            nCreatedLevels++;
          }
          nPairLevels++;
        }
      } else {
        agglomOK = true;
      }
      reduce(nCoarseFaces, sumOp<label>());
    } while (!agglomOK);
    nFaces_[nCreatedLevels] = nCoarseFaces;
    if (!continueAgglomerating(nCoarseFaces)
        || (nCoarseFacesOld ==  nCoarseFaces)) {
      break;
    }
    nCoarseFacesOld = nCoarseFaces;
  }
  compactLevels(nCreatedLevels);
}


mousse::tmp<mousse::labelField> mousse::pairPatchAgglomeration::agglomerateOneLevel
(
  label& nCoarseFaces,
  const bPatch& patch
)
{
  const label nFineFaces = patch.size();
  tmp<labelField> tcoarseCellMap{new labelField(nFineFaces, -1)};
  labelField& coarseCellMap = tcoarseCellMap();
  const labelListList& faceFaces = patch.faceFaces();
  nCoarseFaces = 0;
  FOR_ALL(faceFaces, facei) {
    const labelList& fFaces = faceFaces[facei];
    if (coarseCellMap[facei] < 0) {
      label matchFaceNo = -1;
      label matchFaceNeibNo = -1;
      scalar maxFaceWeight = -GREAT;
      // Check faces to find ungrouped neighbour with largest face weight
      FOR_ALL(fFaces, i) {
        label faceNeig = fFaces[i];
        const edge edgeCommon = edge(facei, faceNeig);
        if (facePairWeight_[edgeCommon] > maxFaceWeight
            && coarseCellMap[faceNeig] < 0
            && facePairWeight_[edgeCommon] != -1.0) {
          // Match found. Pick up all the necessary data
          matchFaceNo = facei;
          matchFaceNeibNo = faceNeig;
          maxFaceWeight = facePairWeight_[edgeCommon];
        }
      }
      if (matchFaceNo >= 0) {
        // Make a new group
        coarseCellMap[matchFaceNo] = nCoarseFaces;
        coarseCellMap[matchFaceNeibNo] = nCoarseFaces;
        nCoarseFaces++;
      } else {
        // No match. Find the best neighbouring cluster and
        // put the cell there
        label clusterMatchFaceNo = -1;
        scalar clusterMaxFaceCoeff = -GREAT;
        FOR_ALL(fFaces, i) {
          label faceNeig = fFaces[i];
          const edge edgeCommon = edge(facei, faceNeig);
          if (facePairWeight_[edgeCommon] > clusterMaxFaceCoeff
              && facePairWeight_[edgeCommon] != -1.0
              && coarseCellMap[faceNeig] > 0) {
            clusterMatchFaceNo = faceNeig;
            clusterMaxFaceCoeff = facePairWeight_[edgeCommon];
          }
        }
        if (clusterMatchFaceNo >= 0) {
          // Add the cell to the best cluster
          coarseCellMap[facei] = coarseCellMap[clusterMatchFaceNo];
        } else {
          // if not create single-cell "clusters" for each
          coarseCellMap[facei] = nCoarseFaces;
          nCoarseFaces++;
        }
      }
    }
  }
  // Check that all faces are part of clusters,
  for (label facei=0; facei<nFineFaces; facei++) {
    if (coarseCellMap[facei] < 0) {
      FATAL_ERROR_IN
      (
        "pairPatchAgglomeration::agglomerateOneLevel "
        "(label&, const bPatch&) "
      )
      << " face " << facei
      << " is not part of a cluster"
      << exit(FatalError);
    }
  }
  return tcoarseCellMap;
}


void mousse::pairPatchAgglomeration::combineLevels(const label curLevel)
{
  label prevLevel = curLevel - 1;
  // Set the previous level nCells to the current
  nFaces_[prevLevel] = nFaces_[curLevel];
  // Map the restrictAddressing from the coarser level into the previous
  // finer level
  const labelList& curResAddr = restrictAddressing_[curLevel];
  labelList& prevResAddr = restrictAddressing_[prevLevel];
  FOR_ALL(prevResAddr, i) {
    prevResAddr[i] = curResAddr[prevResAddr[i]];
  }
  // Delete the restrictAddressing for the coarser level
  restrictAddressing_.set(curLevel, nullptr);
  patchLevels_.set(prevLevel, patchLevels_.set(curLevel, nullptr));
}
