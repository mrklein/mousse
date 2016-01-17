// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef processor_topology_hpp_
#define processor_topology_hpp_
#include "_processor_topology.hpp"
#include "processor_poly_patch.hpp"
namespace mousse
{
typedef ProcessorTopology<polyPatchList, processorPolyPatch> processorTopology;
}  // namespace mousse
#endif
