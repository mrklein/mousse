// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tensor_list.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "token.hpp"


namespace mousse {

DEFINE_COMPOUND_TYPE_NAME(List<tensor>, tensorList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<tensor>, tensorList);

}  // namespace mousse
