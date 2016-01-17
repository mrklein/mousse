// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_reader.hpp"
void mousse::meshReader::calcPointCells() const
{
  static const label UNIT_POINT_CELLS = 12;
  if (pointCellsPtr_)
  {
    FATAL_ERROR_IN("meshReader::calcPointCells() const")
      << "pointCells already calculated"
      << abort(FatalError);
  }
  label nPoints = points_.size();
  pointCellsPtr_ = new labelListList(nPoints);
  labelListList& ptCells = *pointCellsPtr_;
  FOR_ALL(ptCells, i)
  {
    ptCells[i].setSize(UNIT_POINT_CELLS);
  }
  // Initialize the list of labels which will hold the count of the
  // actual number of cells per point during the analysis
  labelList cellCount(nPoints, 0);
  // Note. Unlike the standard point-cell algorithm, which asks the cell for
  // the supporting point labels, we need to work based on the cell faces.
  // This is because some of the faces do not come from the cell shape.
  // It is also advantageous to remove duplicates from the point-cell
  // addressing, because this removes a lot of waste later.
  faceListList& cFaces = cellFaces();
  // For each cell
  FOR_ALL(cFaces, cellI)
  {
    const faceList& faces = cFaces[cellI];
    FOR_ALL(faces, i)
    {
      // For each vertex
      const labelList& labels = faces[i];
      FOR_ALL(labels, j)
      {
        // Set working point label
        label curPoint = labels[j];
        labelList& curPointCells = ptCells[curPoint];
        label curCount = cellCount[curPoint];
        // check if the cell has been added before
        bool found = false;
        for (label f = 0; f < curCount; f++)
        {
          if (curPointCells[f] == cellI)
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          // If the list of pointCells is not big enough, double it
          if (curPointCells.size() <= curCount)
          {
            curPointCells.setSize(curPointCells.size()*2);
          }
          // Enter the cell label in the point's cell list
          curPointCells[curCount] = cellI;
          // Increment the cell count for the point addressed
          cellCount[curPoint]++;
        }
      }
    }
  }
  // report and remove unused points
  // - adjust points, pointCells, and cellFaces accordingly
  label pointI = 0;
  labelList oldToNew(nPoints, -1);
  FOR_ALL(ptCells, i)
  {
    ptCells[i].setSize(cellCount[i]);
    if (cellCount[i] > 0)
    {
      oldToNew[i] = pointI++;
    }
  }
  // report unused points
  if (nPoints > pointI)
  {
    Info<< "removing " << (nPoints - pointI) << " unused points" << endl;
    nPoints = pointI;
    // adjust points and truncate - bend const-ness
    pointField& adjustedPoints = const_cast<pointField&>(points_);
    inplaceReorder(oldToNew, adjustedPoints);
    adjustedPoints.setSize(nPoints);
    // adjust pointCells and truncate
    inplaceReorder(oldToNew, ptCells);
    ptCells.setSize(nPoints);
    // adjust cellFaces - this could be faster
    // For each cell
    FOR_ALL(cFaces, cellI)
    {
      faceList& faces = cFaces[cellI];
      // For each face
      FOR_ALL(faces, i)
      {
        inplaceRenumber(oldToNew, faces[i]);
      }
    }
  }
}
const mousse::labelListList& mousse::meshReader::pointCells() const
{
  if (!pointCellsPtr_)
  {
    calcPointCells();
  }
  return *pointCellsPtr_;
}
