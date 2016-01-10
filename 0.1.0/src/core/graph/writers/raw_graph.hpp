// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::rawGraph
// Description
//   A raw xy graph output
// SourceFiles
//   raw_graph.cpp
#ifndef raw_graph_hpp_
#define raw_graph_hpp_
#include "graph.hpp"
namespace mousse
{
class rawGraph
:
  public graph::writer
{
public:
  //- Runtime type information
  TYPE_NAME("raw");
  //- FileName extension  for this graph format
  static const word ext_;
  // Constructors
    //- Construct null
    rawGraph()
    {}
  //- Destructor
  ~rawGraph()
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
