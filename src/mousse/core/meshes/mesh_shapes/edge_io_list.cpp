// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "edge_io_list.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Static Data Members
DEFINE_COMPOUND_TYPE_NAME(List<edge>, edgeList);
ADD_COMPOUND_TO_RUN_TIME_SELECTION_TABLE(List<edge>, edgeList);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(edgeIOList, "edgeList", 0);
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME(edgeCompactIOList, "edgeCompactList", 0);

}
