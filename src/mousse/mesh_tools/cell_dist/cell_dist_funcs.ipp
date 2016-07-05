// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_dist_funcs.hpp"
#include "poly_mesh.hpp"
#include "poly_boundary_mesh.hpp"


// Member Functions 
template<class Type>
mousse::labelHashSet mousse::cellDistFuncs::getPatchIDs() const
{
  const polyBoundaryMesh& bMesh = mesh().boundaryMesh();
  labelHashSet patchIDs(bMesh.size());
  FOR_ALL(bMesh, patchI) {
    if (isA<Type>(bMesh[patchI])) {
      patchIDs.insert(patchI);
    }
  }
  return patchIDs;
}
