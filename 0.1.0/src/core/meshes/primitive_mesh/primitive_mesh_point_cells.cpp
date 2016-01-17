// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "cell.hpp"
// Private Member Functions 
void mousse::primitiveMesh::calcPointCells() const
{
  // Loop through cells and mark up points
  if (debug)
  {
    Pout<< "primitiveMesh::calcPointCells() : "
      << "calculating pointCells"
      << endl;
    if (debug == -1)
    {
      // For checking calls:abort so we can quickly hunt down
      // origin of call
      FATAL_ERROR_IN("primitiveMesh::calcPointCells()")
        << abort(FatalError);
    }
  }
  // It is an error to attempt to recalculate pointCells
  // if the pointer is already set
  if (pcPtr_)
  {
    FATAL_ERROR_IN("primitiveMesh::calcPointCells() const")
      << "pointCells already calculated"
      << abort(FatalError);
  }
  else
  {
    const cellList& cf = cells();
    // Count number of cells per point
    labelList npc(nPoints(), 0);
    FOR_ALL(cf, cellI)
    {
      const labelList curPoints = cf[cellI].labels(faces());
      FOR_ALL(curPoints, pointI)
      {
        label ptI = curPoints[pointI];
        npc[ptI]++;
      }
    }
    // Size and fill cells per point
    pcPtr_ = new labelListList(npc.size());
    labelListList& pointCellAddr = *pcPtr_;
    FOR_ALL(pointCellAddr, pointI)
    {
      pointCellAddr[pointI].setSize(npc[pointI]);
    }
    npc = 0;
    FOR_ALL(cf, cellI)
    {
      const labelList curPoints = cf[cellI].labels(faces());
      FOR_ALL(curPoints, pointI)
      {
        label ptI = curPoints[pointI];
        pointCellAddr[ptI][npc[ptI]++] = cellI;
      }
    }
  }
}
// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::pointCells() const
{
  if (!pcPtr_)
  {
    calcPointCells();
  }
  return *pcPtr_;
}
const mousse::labelList& mousse::primitiveMesh::pointCells
(
  const label pointI,
  DynamicList<label>& storage
) const
{
  if (hasPointCells())
  {
    return pointCells()[pointI];
  }
  else
  {
    const labelList& own = faceOwner();
    const labelList& nei = faceNeighbour();
    const labelList& pFaces = pointFaces()[pointI];
    storage.clear();
    FOR_ALL(pFaces, i)
    {
      const label faceI = pFaces[i];
      // Append owner
      storage.append(own[faceI]);
      // Append neighbour
      if (faceI < nInternalFaces())
      {
        storage.append(nei[faceI]);
      }
    }
    // Filter duplicates
    if (storage.size() > 1)
    {
      sort(storage);
      label n = 1;
      for (label i = 1; i < storage.size(); i++)
      {
        if (storage[i-1] != storage[i])
        {
          storage[n++] = storage[i];
        }
      }
      // truncate addressed list
      storage.setSize(n);
    }
    return storage;
  }
}
const mousse::labelList& mousse::primitiveMesh::pointCells(const label pointI) const
{
  return pointCells(pointI, labels_);
}
