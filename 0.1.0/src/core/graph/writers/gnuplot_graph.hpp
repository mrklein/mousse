#ifndef CORE_GRAPH_WRITERS_GNUPLOT_GRAPH_HPP_
#define CORE_GRAPH_WRITERS_GNUPLOT_GRAPH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gnuplotGraph
// Description
//   Output in gnuplot (http://www.gnuplot.info) format

#include "graph.hpp"


namespace mousse {

class gnuplotGraph
:
  public graph::writer
{
public:
  //- Runtime type information
  TYPE_NAME("gnuplot");
  //- FileName extension  for this graph format
  static const word ext_;
  // Constructors
    //- Construct null
    gnuplotGraph()
    {}
  //- Destructor
  ~gnuplotGraph()
  {}
  // Member Functions
    // Access
      //- Return the appropriate fileName extension
      //  for this graph format
      const word& ext() const
      {
        return ext_;
      }
    // Write
      void write(const graph&, Ostream& os) const;
};
}  // namespace mousse
#endif
