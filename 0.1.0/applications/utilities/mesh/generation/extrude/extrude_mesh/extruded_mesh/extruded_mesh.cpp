// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "extruded_mesh.hpp"
// Private Member Functions 
bool mousse::extrudedMesh::sameOrder(const face& f, const edge& e)
{
  label i = findIndex(f, e[0]);
  label nextI = (i == f.size()-1 ? 0 : i+1);
  return f[nextI] == e[1];
}
