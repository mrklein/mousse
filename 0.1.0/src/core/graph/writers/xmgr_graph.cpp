// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xmgr_graph.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(xmgrGraph, 0);
const word xmgrGraph::ext_("agr");
typedef graph::writer graphWriter;
ADD_TO_RUN_TIME_SELECTION_TABLE(graphWriter, xmgrGraph, word);

}


// Member Functions 
void mousse::xmgrGraph::write(const graph& g, Ostream& os) const
{
  os << "@title " << g.title() << nl
    << "@xaxis label " << g.xName() << nl
    << "@yaxis label " << g.yName() << endl;
  label fieldI = 0;
  FOR_ALL_CONST_ITER(graph, g, iter)
  {
    os << "@s" << fieldI << " legend "
      << iter()->name() << nl
      << "@target G0.S" << fieldI << nl
      << "@type xy" << endl;
    writeXY(g.x(), *iter(), os);
    os << endl;
    fieldI++;
  }
}
