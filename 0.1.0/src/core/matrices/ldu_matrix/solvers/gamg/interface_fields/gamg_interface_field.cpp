// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gamg_interface_field.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(GAMGInterfaceField, 0);

DEFINE_RUN_TIME_SELECTION_TABLE(GAMGInterfaceField, lduInterface);

DEFINE_RUN_TIME_SELECTION_TABLE(GAMGInterfaceField, lduInterfaceField);

}
