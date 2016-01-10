// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
// Private Member Functions 
void mousse::primitiveMesh::calcCells
(
  cellList& cellFaceAddr,
  const labelUList& own,
  const labelUList& nei,
  const label inNCells
)
{
  label nCells = inNCells;
  if (nCells == -1)
  {
    nCells = -1;
    FOR_ALL(own, faceI)
    {
      nCells = max(nCells, own[faceI]);
    }
    nCells++;
  }
  // 1. Count number of faces per cell
  labelList ncf(nCells, 0);
  FOR_ALL(own, faceI)
  {
    ncf[own[faceI]]++;
  }
  FOR_ALL(nei, faceI)
  {
    if (nei[faceI] >= 0)
    {
      ncf[nei[faceI]]++;
    }
  }
  // Create the storage
  cellFaceAddr.setSize(ncf.size());
  // 2. Size and fill cellFaceAddr
  FOR_ALL(cellFaceAddr, cellI)
  {
    cellFaceAddr[cellI].setSize(ncf[cellI]);
  }
  ncf = 0;
  FOR_ALL(own, faceI)
  {
    label cellI = own[faceI];
    cellFaceAddr[cellI][ncf[cellI]++] = faceI;
  }
  FOR_ALL(nei, faceI)
  {
    label cellI = nei[faceI];
    if (cellI >= 0)
    {
      cellFaceAddr[cellI][ncf[cellI]++] = faceI;
    }
  }
}
void mousse::primitiveMesh::calcCells() const
{
  // Loop through faceCells and mark up neighbours
  if (debug)
  {
    Pout<< "primitiveMesh::calcCells() : calculating cells"
      << endl;
  }
  // It is an error to attempt to recalculate cells
  // if the pointer is already set
  if (cfPtr_)
  {
    FATAL_ERROR_IN("primitiveMesh::calcCells() const")
      << "cells already calculated"
      << abort(FatalError);
  }
  else
  {
    // Create the storage
    cfPtr_ = new cellList(nCells());
    cellList& cellFaceAddr = *cfPtr_;
    calcCells
    (
      cellFaceAddr,
      faceOwner(),
      faceNeighbour(),
      nCells()
    );
  }
}
// Member Functions 
const mousse::cellList& mousse::primitiveMesh::cells() const
{
  if (!cfPtr_)
  {
    calcCells();
  }
  return *cfPtr_;
}
