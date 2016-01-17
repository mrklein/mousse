// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "list_ops.hpp"
// Member Functions 
const mousse::labelListList& mousse::primitiveMesh::pointFaces() const
{
  if (!pfPtr_)
  {
    if (debug)
    {
      Pout<< "primitiveMesh::pointFaces() : "
        << "calculating pointFaces" << endl;
    }
    // Invert faces()
    pfPtr_ = new labelListList(nPoints());
    invertManyToMany(nPoints(), faces(), *pfPtr_);
  }
  return *pfPtr_;
}
