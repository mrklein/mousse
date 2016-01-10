// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::xmgrGraph
// Description
//   Output and \b agr file for \em xmgrace
//   (http://plasma-gate.weizmann.ac.il/Grace/)
// SourceFiles
//   xmgr_graph.cpp
#ifndef xmgr_graph_hpp_
#define xmgr_graph_hpp_
#include "graph.hpp"
namespace mousse
{
class xmgrGraph
:
  public graph::writer
{
public:
  //- Runtime type information
  TYPE_NAME("xmgr");
  //- FileName extension  for this graph format
  static const word ext_;
  // Constructors
    //- Construct null
    xmgrGraph()
    {}
  //- Destructor
  ~xmgrGraph()
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
