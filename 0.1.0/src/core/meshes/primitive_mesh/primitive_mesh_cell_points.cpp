// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "list_ops.hpp"
// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::cellPoints() const
{
  if (!cpPtr_)
  {
    if (debug)
    {
      Pout<< "primitiveMesh::cellPoints() : "
        << "calculating cellPoints" << endl;
      if (debug == -1)
      {
        // For checking calls:abort so we can quickly hunt down
        // origin of call
        FATAL_ERROR_IN("primitiveMesh::cellPoints()")
          << abort(FatalError);
      }
    }
    // Invert pointCells
    cpPtr_ = new labelListList(nCells());
    invertManyToMany(nCells(), pointCells(), *cpPtr_);
  }
  return *cpPtr_;
}
const mousse::labelList& mousse::primitiveMesh::cellPoints
(
  const label cellI,
  DynamicList<label>& storage
) const
{
  if (hasCellPoints())
  {
    return cellPoints()[cellI];
  }
  else
  {
    const faceList& fcs = faces();
    const labelList& cFaces = cells()[cellI];
    labelSet_.clear();
    FOR_ALL(cFaces, i)
    {
      const labelList& f = fcs[cFaces[i]];
      FOR_ALL(f, fp)
      {
        labelSet_.insert(f[fp]);
      }
    }
    storage.clear();
    if (labelSet_.size() > storage.capacity())
    {
      storage.setCapacity(labelSet_.size());
    }
    FOR_ALL_CONST_ITER(labelHashSet, labelSet_, iter)
    {
      storage.append(iter.key());
    }
    return storage;
  }
}
const mousse::labelList& mousse::primitiveMesh::cellPoints(const label cellI) const
{
  return cellPoints(cellI, labels_);
}
