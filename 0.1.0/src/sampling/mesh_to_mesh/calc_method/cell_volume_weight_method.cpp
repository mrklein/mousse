// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_volume_weight_method.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(cellVolumeWeightMethod, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    meshToMeshMethod,
    cellVolumeWeightMethod,
    components
  );
}
// Protected Member Functions 
bool mousse::cellVolumeWeightMethod::findInitialSeeds
(
  const labelList& srcCellIDs,
  const boolList& mapFlag,
  const label startSeedI,
  label& srcSeedI,
  label& tgtSeedI
) const
{
  const cellList& srcCells = src_.cells();
  const faceList& srcFaces = src_.faces();
  const pointField& srcPts = src_.points();
  for (label i = startSeedI; i < srcCellIDs.size(); i++)
  {
    label srcI = srcCellIDs[i];
    if (mapFlag[srcI])
    {
      const pointField
        pts(srcCells[srcI].points(srcFaces, srcPts).xfer());
      FOR_ALL(pts, ptI)
      {
        const point& pt = pts[ptI];
        label tgtI = tgt_.cellTree().findInside(pt);
        if (tgtI != -1 && intersect(srcI, tgtI))
        {
          srcSeedI = srcI;
          tgtSeedI = tgtI;
          return true;
        }
      }
    }
  }
  if (debug)
  {
    Pout<< "could not find starting seed" << endl;
  }
  return false;
}
void mousse::cellVolumeWeightMethod::calculateAddressing
(
  labelListList& srcToTgtCellAddr,
  scalarListList& srcToTgtCellWght,
  labelListList& tgtToSrcCellAddr,
  scalarListList& tgtToSrcCellWght,
  const label srcSeedI,
  const label tgtSeedI,
  const labelList& srcCellIDs,
  boolList& mapFlag,
  label& startSeedI
)
{
  label srcCellI = srcSeedI;
  label tgtCellI = tgtSeedI;
  List<DynamicList<label> > srcToTgtAddr(src_.nCells());
  List<DynamicList<scalar> > srcToTgtWght(src_.nCells());
  List<DynamicList<label> > tgtToSrcAddr(tgt_.nCells());
  List<DynamicList<scalar> > tgtToSrcWght(tgt_.nCells());
  // list of tgt cell neighbour cells
  DynamicList<label> nbrTgtCells(10);
  // list of tgt cells currently visited for srcCellI to avoid multiple hits
  DynamicList<label> visitedTgtCells(10);
  // list to keep track of tgt cells used to seed src cells
  labelList seedCells(src_.nCells(), -1);
  seedCells[srcCellI] = tgtCellI;
  const scalarField& srcVol = src_.cellVolumes();
  do
  {
    nbrTgtCells.clear();
    visitedTgtCells.clear();
    // append initial target cell and neighbours
    nbrTgtCells.append(tgtCellI);
    appendNbrCells(tgtCellI, tgt_, visitedTgtCells, nbrTgtCells);
    do
    {
      tgtCellI = nbrTgtCells.remove();
      visitedTgtCells.append(tgtCellI);
      scalar vol = interVol(srcCellI, tgtCellI);
      // accumulate addressing and weights for valid intersection
      if (vol/srcVol[srcCellI] > tolerance_)
      {
        // store src/tgt cell pair
        srcToTgtAddr[srcCellI].append(tgtCellI);
        srcToTgtWght[srcCellI].append(vol);
        tgtToSrcAddr[tgtCellI].append(srcCellI);
        tgtToSrcWght[tgtCellI].append(vol);
        appendNbrCells(tgtCellI, tgt_, visitedTgtCells, nbrTgtCells);
        // accumulate intersection volume
        V_ += vol;
      }
    }
    while (!nbrTgtCells.empty());
    mapFlag[srcCellI] = false;
    // find new source seed cell
    setNextCells
    (
      startSeedI,
      srcCellI,
      tgtCellI,
      srcCellIDs,
      mapFlag,
      visitedTgtCells,
      seedCells
    );
  }
  while (srcCellI != -1);
  // transfer addressing into persistent storage
  FOR_ALL(srcToTgtCellAddr, i)
  {
    srcToTgtCellAddr[i].transfer(srcToTgtAddr[i]);
    srcToTgtCellWght[i].transfer(srcToTgtWght[i]);
  }
  FOR_ALL(tgtToSrcCellAddr, i)
  {
    tgtToSrcCellAddr[i].transfer(tgtToSrcAddr[i]);
    tgtToSrcCellWght[i].transfer(tgtToSrcWght[i]);
  }
}
void mousse::cellVolumeWeightMethod::setNextCells
(
  label& startSeedI,
  label& srcCellI,
  label& tgtCellI,
  const labelList& srcCellIDs,
  const boolList& mapFlag,
  const DynamicList<label>& visitedCells,
  labelList& seedCells
) const
{
  const labelList& srcNbrCells = src_.cellCells()[srcCellI];
  // set possible seeds for later use by querying all src cell neighbours
  // with all visited target cells
  bool valuesSet = false;
  FOR_ALL(srcNbrCells, i)
  {
    label cellS = srcNbrCells[i];
    if (mapFlag[cellS] && seedCells[cellS] == -1)
    {
      FOR_ALL(visitedCells, j)
      {
        label cellT = visitedCells[j];
        if (intersect(cellS, cellT))
        {
          seedCells[cellS] = cellT;
          if (!valuesSet)
          {
            srcCellI = cellS;
            tgtCellI = cellT;
            valuesSet = true;
          }
        }
      }
    }
  }
  // set next src and tgt cells if not set above
  if (valuesSet)
  {
    return;
  }
  else
  {
    // try to use existing seed
    bool foundNextSeed = false;
    for (label i = startSeedI; i < srcCellIDs.size(); i++)
    {
      label cellS = srcCellIDs[i];
      if (mapFlag[cellS])
      {
        if (!foundNextSeed)
        {
          startSeedI = i;
          foundNextSeed = true;
        }
        if (seedCells[cellS] != -1)
        {
          srcCellI = cellS;
          tgtCellI = seedCells[cellS];
          return;
        }
      }
    }
    // perform new search to find match
    if (debug)
    {
      Pout<< "Advancing front stalled: searching for new "
        << "target cell" << endl;
    }
    bool restart =
      findInitialSeeds
      (
        srcCellIDs,
        mapFlag,
        startSeedI,
        srcCellI,
        tgtCellI
      );
    if (restart)
    {
      // successfully found new starting seed-pair
      return;
    }
  }
  // if we have got to here, there are no more src/tgt cell intersections
  srcCellI = -1;
  tgtCellI = -1;
}
// Constructors 
mousse::cellVolumeWeightMethod::cellVolumeWeightMethod
(
  const polyMesh& src,
  const polyMesh& tgt
)
:
  meshToMeshMethod(src, tgt)
{}
// Destructor
mousse::cellVolumeWeightMethod::~cellVolumeWeightMethod()
{}
// Member Functions 
void mousse::cellVolumeWeightMethod::calculate
(
  labelListList& srcToTgtAddr,
  scalarListList& srcToTgtWght,
  labelListList& tgtToSrcAddr,
  scalarListList& tgtToSrcWght
)
{
  bool ok = initialise
  (
    srcToTgtAddr,
    srcToTgtWght,
    tgtToSrcAddr,
    tgtToSrcWght
  );
  if (!ok)
  {
    return;
  }
  // (potentially) participating source mesh cells
  const labelList srcCellIDs(maskCells());
  // list to keep track of whether src cell can be mapped
  boolList mapFlag(src_.nCells(), false);
  UIndirectList<bool>(mapFlag, srcCellIDs) = true;
  // find initial point in tgt mesh
  label srcSeedI = -1;
  label tgtSeedI = -1;
  label startSeedI = 0;
  bool startWalk =
    findInitialSeeds
    (
      srcCellIDs,
      mapFlag,
      startSeedI,
      srcSeedI,
      tgtSeedI
    );
  if (startWalk)
  {
    calculateAddressing
    (
      srcToTgtAddr,
      srcToTgtWght,
      tgtToSrcAddr,
      tgtToSrcWght,
      srcSeedI,
      tgtSeedI,
      srcCellIDs,
      mapFlag,
      startSeedI
    );
  }
  else
  {
    // if meshes are collocated, after inflating the source mesh bounding
    // box tgt mesh cells may be transferred, but may still not overlap
    // with the source mesh
    return;
  }
}
