// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::jplotGraph
// Description
//   jplot graph output
// SourceFiles
//   jplot_graph.cpp
#ifndef jplot_graph_hpp_
#define jplot_graph_hpp_

#include "graph.hpp"

namespace mousse
{
class jplotGraph
:
  public graph::writer
{
public:
  //- Runtime type information
  TYPE_NAME("jplot");
  //- FileName extension  for this graph format
  static const word ext_;
  // Constructors
    //- Construct null
    jplotGraph()
    {}
  //- Destructor
  ~jplotGraph()
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
