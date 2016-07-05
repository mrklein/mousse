// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tri_surface_region_search.hpp"
#include "indexed_octree.hpp"
#include "tri_surface.hpp"
#include "patch_tools.hpp"
#include "map.hpp"


// Constructors
mousse::triSurfaceRegionSearch::triSurfaceRegionSearch(const triSurface& surface)
:
  triSurfaceSearch{surface},
  indirectRegionPatches_{},
  treeByRegion_{}
{}


mousse::triSurfaceRegionSearch::triSurfaceRegionSearch
(
  const triSurface& surface,
  const dictionary& dict
)
:
  triSurfaceSearch{surface, dict},
  indirectRegionPatches_{},
  treeByRegion_{}
{}


// Destructor
mousse::triSurfaceRegionSearch::~triSurfaceRegionSearch()
{
  clearOut();
}


void mousse::triSurfaceRegionSearch::clearOut()
{
  triSurfaceSearch::clearOut();
  treeByRegion_.clear();
}


// Member Functions
const mousse::PtrList<mousse::triSurfaceRegionSearch::treeType>&
mousse::triSurfaceRegionSearch::treeByRegion() const
{
  if (treeByRegion_.empty()) {
    Map<label> regionSizes;
    FOR_ALL(surface(), fI) {
      const label regionI = surface()[fI].region();
      regionSizes(regionI)++;
    }
    label nRegions = regionSizes.size();
    indirectRegionPatches_.setSize(nRegions);
    treeByRegion_.setSize(nRegions);
    labelListList regionsAddressing{nRegions};
    FOR_ALL(regionsAddressing, regionI) {
      regionsAddressing[regionI] = labelList(regionSizes[regionI], -1);
    }
    labelList nFacesInRegions{nRegions, 0};
    FOR_ALL(surface(), fI) {
      const label regionI = surface()[fI].region();
      regionsAddressing[regionI][nFacesInRegions[regionI]++] = fI;
    }
    FOR_ALL(regionsAddressing, regionI) {
      scalar oldTol = treeType::perturbTol();
      treeType::perturbTol() = tolerance();
      indirectRegionPatches_.set
      (
        regionI,
        new indirectTriSurface
        {
          IndirectList<labelledTri>
          (
            surface(),
            regionsAddressing[regionI]
          ),
          surface().points()
        }
      );
      // Calculate bb without constructing local point numbering.
      treeBoundBox bb{vector::zero, vector::zero};
      if (indirectRegionPatches_[regionI].size()) {
        label nPoints;
        PatchTools::calcBounds
        (
          indirectRegionPatches_[regionI],
          bb,
          nPoints
        );
        // Random number generator. Bit dodgy since not exactly
        // random ;-)
        Random rndGen{65431};
        // Slightly extended bb. Slightly off-centred just so
        // on symmetric geometry there are fewer face/edge
        // aligned items.
        bb = bb.extend(rndGen, 1e-4);
        bb.min() -= point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
        bb.max() += point{ROOTVSMALL, ROOTVSMALL, ROOTVSMALL};
      }
      treeByRegion_.set
      (
        regionI,
        new treeType
        {
          treeDataIndirectTriSurface
          (
            true,
            indirectRegionPatches_[regionI],
            tolerance()
          ),
          bb,
          maxTreeDepth(),  // maxLevel
          10,              // leafsize
          3.0              // duplicity
        }
      );
      treeType::perturbTol() = oldTol;
    }
  }
  return treeByRegion_;
}


void mousse::triSurfaceRegionSearch::findNearest
(
  const pointField& samples,
  const scalarField& nearestDistSqr,
  const labelList& regionIndices,
  List<pointIndexHit>& info
) const
{
  if (regionIndices.empty()) {
    triSurfaceSearch::findNearest(samples, nearestDistSqr, info);
  } else {
    scalar oldTol = treeType::perturbTol();
    treeType::perturbTol() = tolerance();
    const PtrList<treeType>& octrees = treeByRegion();
    info.setSize(samples.size());
    FOR_ALL(octrees, treeI) {
      if (findIndex(regionIndices, treeI) == -1) {
        continue;
      }
      const treeType& octree = octrees[treeI];
      FOR_ALL(samples, i) {
        pointIndexHit currentRegionHit = octree.findNearest
        (
          samples[i],
          nearestDistSqr[i],
          treeDataIndirectTriSurface::findNearestOp(octree)
        );
        if (currentRegionHit.hit()
            && (!info[i].hit() ||
                (magSqr(currentRegionHit.hitPoint() - samples[i])
                 < magSqr(info[i].hitPoint() - samples[i])))) {
          info[i] = currentRegionHit;
        }
      }
    }
    treeType::perturbTol() = oldTol;
  }
}

