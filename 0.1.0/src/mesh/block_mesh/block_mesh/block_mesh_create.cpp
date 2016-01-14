// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "block_mesh.hpp"
#include "cell_modeller.hpp"
// Private Member Functions 
void mousse::blockMesh::createPoints() const
{
  const blockList& blocks = *this;
  if (verboseOutput)
  {
    Info<< "Creating points with scale " << scaleFactor_ << endl;
  }
  //
  // generate points
  //
  points_.clear();
  points_.setSize(nPoints_);
  FOR_ALL(blocks, blockI)
  {
    const pointField& blockPoints = blocks[blockI].points();
    if (verboseOutput)
    {
      const Vector<label>& density = blocks[blockI].meshDensity();
      label v0 = blocks[blockI].vtxLabel(0, 0, 0);
      label vi1 = blocks[blockI].vtxLabel(1, 0, 0);
      scalar diStart = mag(blockPoints[vi1]-blockPoints[v0]);
      label vinM1 = blocks[blockI].vtxLabel(density.x()-1, 0, 0);
      label vin = blocks[blockI].vtxLabel(density.x(), 0, 0);
      scalar diFinal = mag(blockPoints[vin]-blockPoints[vinM1]);
      label vj1 = blocks[blockI].vtxLabel(0, 1, 0);
      scalar djStart = mag(blockPoints[vj1]-blockPoints[v0]);
      label vjnM1 = blocks[blockI].vtxLabel(0, density.y()-1, 0);
      label vjn = blocks[blockI].vtxLabel(0, density.y(), 0);
      scalar djFinal = mag(blockPoints[vjn]-blockPoints[vjnM1]);
      label vk1 = blocks[blockI].vtxLabel(0, 0, 1);
      scalar dkStart = mag(blockPoints[vk1]-blockPoints[v0]);
      label vknM1 = blocks[blockI].vtxLabel(0, 0, density.z()-1);
      label vkn = blocks[blockI].vtxLabel(0, 0, density.z());
      scalar dkFinal = mag(blockPoints[vkn]-blockPoints[vknM1]);
      Info<< "    Block " << blockI << " cell size :" << nl
        << "        i : " << scaleFactor_*diStart << " .. "
        << scaleFactor_*diFinal << nl
        << "        j : " << scaleFactor_*djStart << " .. "
        << scaleFactor_*djFinal << nl
        << "        k : " << scaleFactor_*dkStart << " .. "
        << scaleFactor_*dkFinal << nl
        << endl;
    }
    FOR_ALL(blockPoints, blockPointI)
    {
      points_
      [
        mergeList_
        [
          blockOffsets_[blockI] + blockPointI
        ]
      ] = scaleFactor_ * blockPoints[blockPointI];
    }
  }
}
void mousse::blockMesh::createCells() const
{
  const blockList& blocks = *this;
  const cellModel& hex = *(cellModeller::lookup("hex"));
  if (verboseOutput)
  {
    Info<< "Creating cells" << endl;
  }
  //
  // generate cells
  //
  cells_.clear();
  cells_.setSize(nCells_);
  label cellLabel = 0;
  FOR_ALL(blocks, blockI)
  {
    const labelListList& blockCells = blocks[blockI].cells();
    FOR_ALL(blockCells, blockCellI)
    {
      labelList cellPoints(blockCells[blockCellI].size());
      FOR_ALL(cellPoints, cellPointI)
      {
        cellPoints[cellPointI] =
          mergeList_
          [
            blockCells[blockCellI][cellPointI]
           + blockOffsets_[blockI]
          ];
      }
      // Construct collapsed cell and add to list
      cells_[cellLabel] = cellShape(hex, cellPoints, true);
      cellLabel++;
    }
  }
}
mousse::faceList mousse::blockMesh::createPatchFaces
(
  const polyPatch& patchTopologyFaces
) const
{
  const blockList& blocks = *this;
  labelList blockLabels = patchTopologyFaces.polyPatch::faceCells();
  label nFaces = 0;
  FOR_ALL(patchTopologyFaces, patchTopologyFaceLabel)
  {
    const label blockI = blockLabels[patchTopologyFaceLabel];
    faceList blockFaces = blocks[blockI].blockShape().faces();
    FOR_ALL(blockFaces, blockFaceLabel)
    {
      if
      (
        blockFaces[blockFaceLabel]
      == patchTopologyFaces[patchTopologyFaceLabel]
      )
      {
        nFaces +=
          blocks[blockI].boundaryPatches()[blockFaceLabel].size();
      }
    }
  }
  faceList patchFaces(nFaces);
  face quadFace(4);
  label faceLabel = 0;
  FOR_ALL(patchTopologyFaces, patchTopologyFaceLabel)
  {
    const label blockI = blockLabels[patchTopologyFaceLabel];
    faceList blockFaces = blocks[blockI].blockShape().faces();
    FOR_ALL(blockFaces, blockFaceLabel)
    {
      if
      (
        blockFaces[blockFaceLabel]
        == patchTopologyFaces[patchTopologyFaceLabel]
      )
      {
        const labelListList& blockPatchFaces =
          blocks[blockI].boundaryPatches()[blockFaceLabel];
        FOR_ALL(blockPatchFaces, blockFaceLabel)
        {
          // Lookup the face points
          // and collapse duplicate point labels
          quadFace[0] =
            mergeList_
            [
              blockPatchFaces[blockFaceLabel][0]
             + blockOffsets_[blockI]
            ];
          label nUnique = 1;
          for
          (
            label facePointLabel = 1;
            facePointLabel < 4;
            facePointLabel++
          )
          {
            quadFace[nUnique] =
              mergeList_
              [
                blockPatchFaces[blockFaceLabel][facePointLabel]
               + blockOffsets_[blockI]
              ];
            if (quadFace[nUnique] != quadFace[nUnique-1])
            {
              nUnique++;
            }
          }
          if (quadFace[nUnique-1] == quadFace[0])
          {
            nUnique--;
          }
          if (nUnique == 4)
          {
            patchFaces[faceLabel++] = quadFace;
          }
          else if (nUnique == 3)
          {
            patchFaces[faceLabel++] = face
            (
              labelList::subList(quadFace, 3)
            );
          }
          // else the face has collapsed to an edge or point
        }
      }
    }
  }
  patchFaces.setSize(faceLabel);
  return patchFaces;
}
void mousse::blockMesh::createPatches() const
{
  const polyPatchList& topoPatches = topology().boundaryMesh();
  if (verboseOutput)
  {
    Info<< "Creating patches" << endl;
  }
  //
  // generate points
  //
  patches_.clear();
  patches_.setSize(topoPatches.size());
  FOR_ALL(topoPatches, patchI)
  {
    patches_[patchI] = createPatchFaces(topoPatches[patchI]);
  }
}
void mousse::blockMesh::clearGeom()
{
  blockList& blocks = *this;
  FOR_ALL(blocks, blockI)
  {
    blocks[blockI].clearGeom();
  }
}
