#ifndef MESH_BLOCK_MESH_CURVED_EDGES_ARC_EDGE_HPP_
#define MESH_BLOCK_MESH_CURVED_EDGES_ARC_EDGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::arcEdge
// Description
//   Defines the arcEdge of a circle in terms of 3 points on its circumference
// SourceFiles
//   arc_edge.cpp
#include "curved_edge.hpp"
#include "cylindrical_cs.hpp"
namespace mousse
{
class arcEdge
:
  public curvedEdge
{
  // Private data
    point p1_, p2_, p3_;
    cylindricalCS cs_;
    scalar angle_;
    scalar radius_;
  // Private Member Functions
    //- Calculate the coordinate system, angle and radius
    cylindricalCS calcAngle();
public:
  //- Runtime type information
  TYPE_NAME("arc");
  // Constructors
    //- Construct from components
    arcEdge
    (
      const pointField& points,
      const label start, const label end,
      const point& pMid
    );
    //- Construct from Istream setting pointsList
    arcEdge(const pointField& points, Istream&);
    //- Disallow default bitwise copy construct
    arcEdge(const arcEdge&) = delete;
    //- Disallow default bitwise assignment
    arcEdge& operator=(const arcEdge&) = delete;
  //- Destructor
  virtual ~arcEdge(){}
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    point position(const scalar) const;
    //- Return the length of the curve
    scalar length() const;
};
}  // namespace mousse
#endif
