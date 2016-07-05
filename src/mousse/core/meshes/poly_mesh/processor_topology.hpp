#ifndef CORE_MESHES_POLY_MESH_PROCESSOR_TOPOLOGY_HPP_
#define CORE_MESHES_POLY_MESH_PROCESSOR_TOPOLOGY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_processor_topology.hpp"
#include "processor_poly_patch.hpp"


namespace mousse {

typedef ProcessorTopology<polyPatchList, processorPolyPatch> processorTopology;

}  // namespace mousse

#endif
