#ifndef MESH_BLOCK_MESH_CURVED_EDGES_LINE_EDGE_HPP_
#define MESH_BLOCK_MESH_CURVED_EDGES_LINE_EDGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::lineEdge
// Description
//   A straight edge between the start point and the end point.
// SourceFiles
//   line_edge.cpp
#include "curved_edge.hpp"
namespace mousse
{
class lineEdge
:
  public curvedEdge
{
public:
  //- Runtime type information
  TYPE_NAME("line");
  // Constructors
    //- Construct from components
    lineEdge(const pointField&, const label start, const label end);
    //- Construct from Istream with a pointField
    lineEdge(const pointField&, Istream&);
    //- Destructor
    virtual ~lineEdge();
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    point position(const scalar) const;
    //- Return the length of the curve
    scalar length() const;
};
}  // namespace mousse
#endif
