// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   map_lagrangian.cpp
#ifndef map_lagrangian_hpp_
#define map_lagrangian_hpp_
#include "mesh_to_mesh0.hpp"
namespace mousse
{
//- Maps lagrangian positions and fields
void mapLagrangian(const meshToMesh0& meshToMesh0Interp);
}  // namespace mousse
#endif
