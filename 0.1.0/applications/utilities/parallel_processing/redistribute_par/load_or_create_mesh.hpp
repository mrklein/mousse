#ifndef UTILITIES_PARALLEL_PROCESSING_REDISTRIBUTE_PAR_LOAD_OR_CREATE_MESH_HPP_
#define UTILITIES_PARALLEL_PROCESSING_REDISTRIBUTE_PAR_LOAD_OR_CREATE_MESH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   load_or_create_mesh.cpp

#include "fv_mesh.hpp"

namespace mousse
{
//- Load (if it exists) or create zero cell mesh given an IOobject:
//      name     : regionName
//      instance : exact directory where to find mesh (i.e. does not
//                 do a findInstance
autoPtr<fvMesh> loadOrCreateMesh(const IOobject& io);
}
#endif
