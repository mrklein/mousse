// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "label_list.hpp"
#include "label_io_list.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "token.hpp"


namespace mousse {

DEFINE_COMPOUND_TYPE_NAME(List<label>, labelList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<label>, labelList);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(labelIOList, "labelList", 0);

}
