// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raw_graph.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(rawGraph, 0);
const word rawGraph::ext_("xy");
typedef graph::writer graphWriter;
ADD_TO_RUN_TIME_SELECTION_TABLE(graphWriter, rawGraph, word);

}

// Member Functions 
void mousse::rawGraph::write(const graph& g, Ostream& os) const
{
  g.writeTable(os);
}
