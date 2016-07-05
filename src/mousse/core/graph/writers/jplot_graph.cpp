// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "jplot_graph.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(jplotGraph, 0);
const word jplotGraph::ext_("dat");
typedef graph::writer graphWriter;
ADD_TO_RUN_TIME_SELECTION_TABLE(graphWriter, jplotGraph, word);

}

// Member Functions 
void mousse::jplotGraph::write(const graph& g, Ostream& os) const
{
  os << "# JPlot file" << nl << "# column 1: " << g.xName() << endl;
  label fieldI = 0;
  FOR_ALL_CONST_ITER(graph, g, iter) {
    os << "# column " << fieldI + 2 << ": " << (*iter()).name() << endl;
    fieldI++;
  }
  g.writeTable(os);
}
