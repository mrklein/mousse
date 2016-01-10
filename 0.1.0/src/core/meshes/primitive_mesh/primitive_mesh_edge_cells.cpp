// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "list_ops.hpp"
// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::edgeCells() const
{
  if (!ecPtr_)
  {
    if (debug)
    {
      Pout<< "primitiveMesh::edgeCells() : calculating edgeCells" << endl;
      if (debug == -1)
      {
        // For checking calls:abort so we can quickly hunt down
        // origin of call
        FATAL_ERROR_IN("primitiveMesh::edgeCells()")
          << abort(FatalError);
      }
    }
    // Invert cellEdges
    ecPtr_ = new labelListList(nEdges());
    invertManyToMany(nEdges(), cellEdges(), *ecPtr_);
  }
  return *ecPtr_;
}
const mousse::labelList& mousse::primitiveMesh::edgeCells
(
  const label edgeI,
  DynamicList<label>& storage
) const
{
  if (hasEdgeCells())
  {
    return edgeCells()[edgeI];
  }
  else
  {
    const labelList& own = faceOwner();
    const labelList& nei = faceNeighbour();
    // Construct edgeFaces
    DynamicList<label> eFacesStorage;
    const labelList& eFaces = edgeFaces(edgeI, eFacesStorage);
    storage.clear();
    // Do quadratic insertion.
    FOR_ALL(eFaces, i)
    {
      label faceI = eFaces[i];
      {
        label ownCellI = own[faceI];
        // Check if not already in storage
        FOR_ALL(storage, j)
        {
          if (storage[j] == ownCellI)
          {
            ownCellI = -1;
            break;
          }
        }
        if (ownCellI != -1)
        {
          storage.append(ownCellI);
        }
      }
      if (isInternalFace(faceI))
      {
        label neiCellI = nei[faceI];
        FOR_ALL(storage, j)
        {
          if (storage[j] == neiCellI)
          {
            neiCellI = -1;
            break;
          }
        }
        if (neiCellI != -1)
        {
          storage.append(neiCellI);
        }
      }
    }
    return storage;
  }
}
const mousse::labelList& mousse::primitiveMesh::edgeCells(const label edgeI) const
{
  return edgeCells(edgeI, labels_);
}
