#ifndef MESH_BLOCK_MESH_CURVED_EDGES_POLY_LINE_EDGE_HPP_
#define MESH_BLOCK_MESH_CURVED_EDGES_POLY_LINE_EDGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polyLineEdge
// Description
//   A curvedEdge defined in terms of a series of straight line segments.

#include "curved_edge.hpp"
#include "poly_line.hpp"


namespace mousse {

class polyLineEdge
:
  public curvedEdge,
  public polyLine
{
public:
  // Static data members
  TYPE_NAME("polyLine");
  // Constructors
    //- Construct from components
    polyLineEdge
    (
      const pointField&,
      const label start,
      const label end,
      const pointField& otherPoints
    );
    //- Construct from Istream
    polyLineEdge(const pointField&, Istream&);
    //- Disallow default bitwise copy construct
    polyLineEdge(const polyLineEdge&) = delete;
    //- Disallow default bitwise assignment
    polyLineEdge& operator=(const polyLineEdge&) = delete;
    //- Destructor
    virtual ~polyLineEdge();
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    point position(const scalar lambda) const;
    //- Return the length of the curve
    scalar length() const;
};

}  // namespace mousse

#endif

