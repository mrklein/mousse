// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tet_overlap_volume.hpp"
#include "tetrahedron.hpp"
#include "tet_points.hpp"
#include "poly_mesh.hpp"
#include "ofstream.hpp"
#include "tree_bound_box.hpp"
#include "indexed_octree.hpp"
#include "tree_data_cell.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(tetOverlapVolume, 0);
}
// Constructors 
mousse::tetOverlapVolume::tetOverlapVolume()
{}
// Private Member Functions 
mousse::scalar mousse::tetOverlapVolume::tetTetOverlapVol
(
  const tetPoints& tetA,
  const tetPoints& tetB
) const
{
  static tetPointRef::tetIntersectionList insideTets;
  label nInside = 0;
  static tetPointRef::tetIntersectionList cutInsideTets;
  label nCutInside = 0;
  tetPointRef::storeOp inside(insideTets, nInside);
  tetPointRef::storeOp cutInside(cutInsideTets, nCutInside);
  tetPointRef::sumVolOp volInside;
  tetPointRef::dummyOp outside;
  if ((tetA.tet().mag() < SMALL*SMALL) || (tetB.tet().mag() < SMALL*SMALL))
  {
    return 0.0;
  }
  // face0
  plane pl0(tetB[1], tetB[3], tetB[2]);
  tetA.tet().sliceWithPlane(pl0, cutInside, outside);
  if (nCutInside == 0)
  {
    return 0.0;
  }
  // face1
  plane pl1(tetB[0], tetB[2], tetB[3]);
  nInside = 0;
  for (label i = 0; i < nCutInside; i++)
  {
    const tetPointRef t = cutInsideTets[i].tet();
    t.sliceWithPlane(pl1, inside, outside);
  }
  if (nInside == 0)
  {
    return 0.0;
  }
  // face2
  plane pl2(tetB[0], tetB[3], tetB[1]);
  nCutInside = 0;
  for (label i = 0; i < nInside; i++)
  {
    const tetPointRef t = insideTets[i].tet();
    t.sliceWithPlane(pl2, cutInside, outside);
  }
  if (nCutInside == 0)
  {
    return 0.0;
  }
  // face3
  plane pl3(tetB[0], tetB[1], tetB[2]);
  for (label i = 0; i < nCutInside; i++)
  {
    const tetPointRef t = cutInsideTets[i].tet();
    t.sliceWithPlane(pl3, volInside, outside);
  }
  return volInside.vol_;
}
mousse::treeBoundBox mousse::tetOverlapVolume::pyrBb
(
  const pointField& points,
  const face& f,
  const point& fc
) const
{
  treeBoundBox bb(fc, fc);
  forAll(f, fp)
  {
    const point& pt = points[f[fp]];
    bb.min() = min(bb.min(), pt);
    bb.max() = max(bb.max(), pt);
  }
  return bb;
}
// Public Member Functions 
bool mousse::tetOverlapVolume::cellCellOverlapMinDecomp
(
  const primitiveMesh& meshA,
  const label cellAI,
  const primitiveMesh& meshB,
  const label cellBI,
  const treeBoundBox& cellBbB,
  const scalar threshold
) const
{
  const cell& cFacesA = meshA.cells()[cellAI];
  const point& ccA = meshA.cellCentres()[cellAI];
  const cell& cFacesB = meshB.cells()[cellBI];
  const point& ccB = meshB.cellCentres()[cellBI];
  scalar vol = 0.0;
  forAll(cFacesA, cFA)
  {
    label faceAI = cFacesA[cFA];
    const face& fA = meshA.faces()[faceAI];
    const treeBoundBox pyrA = pyrBb(meshA.points(), fA, ccA);
    if (!pyrA.overlaps(cellBbB))
    {
      continue;
    }
    bool ownA = (meshA.faceOwner()[faceAI] == cellAI);
    label tetBasePtAI = 0;
    const point& tetBasePtA = meshA.points()[fA[tetBasePtAI]];
    for (label tetPtI = 1; tetPtI < fA.size() - 1; tetPtI++)
    {
      label facePtAI = (tetPtI + tetBasePtAI) % fA.size();
      label otherFacePtAI = fA.fcIndex(facePtAI);
      label pt0I = -1;
      label pt1I = -1;
      if (ownA)
      {
        pt0I = fA[facePtAI];
        pt1I = fA[otherFacePtAI];
      }
      else
      {
        pt0I = fA[otherFacePtAI];
        pt1I = fA[facePtAI];
      }
      const tetPoints tetA
      (
        ccA,
        tetBasePtA,
        meshA.points()[pt0I],
        meshA.points()[pt1I]
      );
      const treeBoundBox tetABb(tetA.bounds());
      // Loop over tets of cellB
      forAll(cFacesB, cFB)
      {
        label faceBI = cFacesB[cFB];
        const face& fB = meshB.faces()[faceBI];
        const treeBoundBox pyrB = pyrBb(meshB.points(), fB, ccB);
        if (!pyrB.overlaps(pyrA))
        {
          continue;
        }
        bool ownB = (meshB.faceOwner()[faceBI] == cellBI);
        label tetBasePtBI = 0;
        const point& tetBasePtB = meshB.points()[fB[tetBasePtBI]];
        for (label tetPtI = 1; tetPtI < fB.size() - 1; tetPtI++)
        {
          label facePtBI = (tetPtI + tetBasePtBI) % fB.size();
          label otherFacePtBI = fB.fcIndex(facePtBI);
          label pt0I = -1;
          label pt1I = -1;
          if (ownB)
          {
            pt0I = fB[facePtBI];
            pt1I = fB[otherFacePtBI];
          }
          else
          {
            pt0I = fB[otherFacePtBI];
            pt1I = fB[facePtBI];
          }
          const tetPoints tetB
          (
            ccB,
            tetBasePtB,
            meshB.points()[pt0I],
            meshB.points()[pt1I]
          );
          if (!tetB.bounds().overlaps(tetABb))
          {
            continue;
          }
          vol += tetTetOverlapVol(tetA, tetB);
          if (vol > threshold)
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}
mousse::scalar mousse::tetOverlapVolume::cellCellOverlapVolumeMinDecomp
(
  const primitiveMesh& meshA,
  const label cellAI,
  const primitiveMesh& meshB,
  const label cellBI,
  const treeBoundBox& cellBbB
) const
{
  const cell& cFacesA = meshA.cells()[cellAI];
  const point& ccA = meshA.cellCentres()[cellAI];
  const cell& cFacesB = meshB.cells()[cellBI];
  const point& ccB = meshB.cellCentres()[cellBI];
  scalar vol = 0.0;
  forAll(cFacesA, cFA)
  {
    label faceAI = cFacesA[cFA];
    const face& fA = meshA.faces()[faceAI];
    const treeBoundBox pyrA = pyrBb(meshA.points(), fA, ccA);
    if (!pyrA.overlaps(cellBbB))
    {
      continue;
    }
    bool ownA = (meshA.faceOwner()[faceAI] == cellAI);
    label tetBasePtAI = 0;
    const point& tetBasePtA = meshA.points()[fA[tetBasePtAI]];
    for (label tetPtI = 1; tetPtI < fA.size() - 1; tetPtI++)
    {
      label facePtAI = (tetPtI + tetBasePtAI) % fA.size();
      label otherFacePtAI = fA.fcIndex(facePtAI);
      label pt0I = -1;
      label pt1I = -1;
      if (ownA)
      {
        pt0I = fA[facePtAI];
        pt1I = fA[otherFacePtAI];
      }
      else
      {
        pt0I = fA[otherFacePtAI];
        pt1I = fA[facePtAI];
      }
      const tetPoints tetA
      (
        ccA,
        tetBasePtA,
        meshA.points()[pt0I],
        meshA.points()[pt1I]
      );
      const treeBoundBox tetABb(tetA.bounds());
      // Loop over tets of cellB
      forAll(cFacesB, cFB)
      {
        label faceBI = cFacesB[cFB];
        const face& fB = meshB.faces()[faceBI];
        const treeBoundBox pyrB = pyrBb(meshB.points(), fB, ccB);
        if (!pyrB.overlaps(pyrA))
        {
          continue;
        }
        bool ownB = (meshB.faceOwner()[faceBI] == cellBI);
        label tetBasePtBI = 0;
        const point& tetBasePtB = meshB.points()[fB[tetBasePtBI]];
        for (label tetPtI = 1; tetPtI < fB.size() - 1; tetPtI++)
        {
          label facePtBI = (tetPtI + tetBasePtBI) % fB.size();
          label otherFacePtBI = fB.fcIndex(facePtBI);
          label pt0I = -1;
          label pt1I = -1;
          if (ownB)
          {
            pt0I = fB[facePtBI];
            pt1I = fB[otherFacePtBI];
          }
          else
          {
            pt0I = fB[otherFacePtBI];
            pt1I = fB[facePtBI];
          }
          const tetPoints tetB
          (
            ccB,
            tetBasePtB,
            meshB.points()[pt0I],
            meshB.points()[pt1I]
          );
          if (!tetB.bounds().overlaps(tetABb))
          {
            continue;
          }
          vol += tetTetOverlapVol(tetA, tetB);
        }
      }
    }
  }
  return vol;
}
mousse::labelList mousse::tetOverlapVolume::overlappingCells
(
  const polyMesh& fromMesh,
  const polyMesh& toMesh,
  const label iTo
) const
{
  const indexedOctree<treeDataCell>& treeA = fromMesh.cellTree();
  treeBoundBox bbB(toMesh.points(), toMesh.cellPoints()[iTo]);
  return treeA.findBox(bbB);
}
