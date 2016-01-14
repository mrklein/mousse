// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::splineEdge
// Description
//   A curvedEdge interface for Catmull-Rom splines.
// SourceFiles
//   spline_edge.cpp
#ifndef spline_edge_hpp_
#define spline_edge_hpp_
#include "curved_edge.hpp"
#include "catmull_rom_spline.hpp"
namespace mousse
{
class splineEdge
:
  public curvedEdge,
  public CatmullRomSpline
{
public:
  //- Runtime type information
  TYPE_NAME("spline");
  // Constructors
    //- Construct from components
    splineEdge
    (
      const pointField&,
      const label start,
      const label end,
      const pointField& internalPoints
    );
    //- Construct from Istream, setting pointsList
    splineEdge(const pointField&, Istream&);
    //- Disallow default bitwise copy construct
    splineEdge(const splineEdge&) = delete;
    //- Disallow default bitwise assignment
    splineEdge& operator=(const splineEdge&) = delete;
  //- Destructor
  virtual ~splineEdge();
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    virtual point position(const scalar) const;
    //- Return the length of the spline curve (not implemented)
    virtual scalar length() const;
};
}  // namespace mousse
#endif
