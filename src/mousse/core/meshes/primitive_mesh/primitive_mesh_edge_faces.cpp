// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "list_ops.hpp"


// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::edgeFaces() const
{
  if (!efPtr_) {
    if (debug) {
      Pout<< "primitiveMesh::edgeFaces() : calculating edgeFaces" << endl;
      if (debug == -1) {
        // For checking calls:abort so we can quickly hunt down
        // origin of call
        FATAL_ERROR_IN("primitiveMesh::edgeFaces()")
          << abort(FatalError);
      }
    }
    // Invert faceEdges
    efPtr_ = new labelListList(nEdges());
    invertManyToMany(nEdges(), faceEdges(), *efPtr_);
  }
  return *efPtr_;
}


const mousse::labelList& mousse::primitiveMesh::edgeFaces
(
  const label edgeI,
  DynamicList<label>& storage
) const
{
  if (hasEdgeFaces()) {
    return edgeFaces()[edgeI];
  } else {
    // Use the fact that pointEdges are sorted in incrementing edge order
    const edge& e = edges()[edgeI];
    const labelList& pFaces0 = pointFaces()[e[0]];
    const labelList& pFaces1 = pointFaces()[e[1]];
    label i0 = 0;
    label i1 = 0;
    storage.clear();
    while (i0 < pFaces0.size() && i1 < pFaces1.size()) {
      if (pFaces0[i0] < pFaces1[i1]) {
        ++i0;
      } else if (pFaces0[i0] > pFaces1[i1]) {
        ++i1;
      } else {
        // Equal. Append.
        storage.append(pFaces0[i0]);
        ++i0;
        ++i1;
      }
    }
    return storage;
  }
}


const mousse::labelList& mousse::primitiveMesh::edgeFaces(const label edgeI) const
{
  return edgeFaces(edgeI, labels_);
}

