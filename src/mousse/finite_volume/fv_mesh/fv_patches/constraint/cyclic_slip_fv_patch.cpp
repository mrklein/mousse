// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cyclic_slip_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(cyclicSlipFvPatch, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(fvPatch, cyclicSlipFvPatch, polyPatch);

}  // namespace mousse
