// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "processor_cyclic_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "transform_field.hpp"
namespace mousse
{
defineTypeNameAndDebug(processorCyclicFvPatch, 0);
addToRunTimeSelectionTable(fvPatch, processorCyclicFvPatch, polyPatch);
}  // namespace mousse
