#ifndef UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_MAP_LAGRANGIAN_HPP_
#define UTILITIES_PRE_PROCESSING_MAP_FIELDS_PAR_MAP_LAGRANGIAN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh.hpp"


namespace mousse {

//- Maps lagrangian positions and fields
void mapLagrangian(const meshToMesh& interp);

}  // namespace mousse

#endif

