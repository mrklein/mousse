// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
// Private Member Functions 
void mousse::polyMesh::initMesh()
{
  if (debug)
  {
    Info<< "void polyMesh::initMesh() : "
      << "initialising primitiveMesh" << endl;
  }
  // For backward compatibility check if the neighbour array is the same
  // length as the owner and shrink to remove the -1s padding
  if (neighbour_.size() == owner_.size())
  {
    label nInternalFaces = 0;
    FOR_ALL(neighbour_, faceI)
    {
      if (neighbour_[faceI] == -1)
      {
        break;
      }
      else
      {
        nInternalFaces++;
      }
    }
    neighbour_.setSize(nInternalFaces);
  }
  label nCells = -1;
  FOR_ALL(owner_, facei)
  {
    if (owner_[facei] < 0)
    {
      FATAL_ERROR_IN("polyMesh::initMesh()")
        << "Illegal cell label " << owner_[facei]
        << " in neighbour addressing for face " << facei
        << exit(FatalError);
    }
    nCells = max(nCells, owner_[facei]);
  }
  // The neighbour array may or may not be the same length as the owner
  FOR_ALL(neighbour_, facei)
  {
    if (neighbour_[facei] < 0)
    {
      FATAL_ERROR_IN("polyMesh::initMesh()")
        << "Illegal cell label " << neighbour_[facei]
        << " in neighbour addressing for face " << facei
        << exit(FatalError);
    }
    nCells = max(nCells, neighbour_[facei]);
  }
  nCells++;
  // Reset the primitiveMesh with the sizes of the primitive arrays
  primitiveMesh::reset
  (
    points_.size(),
    neighbour_.size(),
    owner_.size(),
    nCells
  );
  string meshInfo =
    "nPoints:" + mousse::name(nPoints())
   + "  nCells:" + mousse::name(this->nCells())
   + "  nFaces:" + mousse::name(nFaces())
   + "  nInternalFaces:" + mousse::name(nInternalFaces());
  owner_.note() = meshInfo;
  neighbour_.note() = meshInfo;
}
void mousse::polyMesh::initMesh(cellList& c)
{
  if (debug)
  {
    Info<< "void polyMesh::initMesh(cellList& c) : "
      << "calculating owner-neighbour arrays" << endl;
  }
  owner_.setSize(faces_.size(), -1);
  neighbour_.setSize(faces_.size(), -1);
  boolList markedFaces(faces_.size(), false);
  label nInternalFaces = 0;
  FOR_ALL(c, cellI)
  {
    // get reference to face labels for current cell
    const labelList& cellfaces = c[cellI];
    FOR_ALL(cellfaces, faceI)
    {
      if (cellfaces[faceI] < 0)
      {
        FATAL_ERROR_IN("polyMesh::initMesh(cellList&)")
          << "Illegal face label " << cellfaces[faceI]
          << " in cell " << cellI
          << exit(FatalError);
      }
      if (!markedFaces[cellfaces[faceI]])
      {
        // First visit: owner
        owner_[cellfaces[faceI]] = cellI;
        markedFaces[cellfaces[faceI]] = true;
      }
      else
      {
        // Second visit: neighbour
        neighbour_[cellfaces[faceI]] = cellI;
        nInternalFaces++;
      }
    }
  }
  // The neighbour array is initialised with the same length as the owner
  // padded with -1s and here it is truncated to the correct size of the
  // number of internal faces.
  neighbour_.setSize(nInternalFaces);
  // Reset the primitiveMesh
  primitiveMesh::reset
  (
    points_.size(),
    neighbour_.size(),
    owner_.size(),
    c.size(),
    c
  );
  string meshInfo =
    "nPoints: " + mousse::name(nPoints())
   + " nCells: " + mousse::name(nCells())
   + " nFaces: " + mousse::name(nFaces())
   + " nInternalFaces: " + mousse::name(this->nInternalFaces());
  owner_.note() = meshInfo;
  neighbour_.note() = meshInfo;
}
