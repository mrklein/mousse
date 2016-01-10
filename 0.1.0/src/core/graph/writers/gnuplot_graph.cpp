// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gnuplot_graph.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{

DEFINE_TYPE_NAME_AND_DEBUG(gnuplotGraph, 0);
const word gnuplotGraph::ext_("gplt");
typedef graph::writer graphWriter;
ADD_TO_RUN_TIME_SELECTION_TABLE(graphWriter, gnuplotGraph, word);

}
// Member Functions 
void mousse::gnuplotGraph::write(const graph& g, Ostream& os) const
{
  os  << "#set term postscript color" << endl
    << "set output \"" << word(g.title()) << ".ps\"" << endl
    << "set title " << g.title() << " 0,0" << endl << "show title" << endl
    << "set xlabel " << g.xName() << " 0,0" << endl << "show xlabel" << endl
    << "set ylabel " << g.yName() << " 0,0" << endl << "show ylabel" << endl
    << "plot";
  bool firstField = true;
  FOR_ALL_CONST_ITER(graph, g, iter)
  {
    if (!firstField)
    {
      os << ',';
    }
    firstField = false;
    os  << "'-' title " << iter()->name() << " with lines";
  }
  os << "; pause -1" << endl;
  FOR_ALL_CONST_ITER(graph, g, iter)
  {
    os  << endl;
    writeXY(g.x(), *iter(), os);
  }
}
