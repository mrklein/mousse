// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::BSplineEdge
// Description
//   A curvedEdge interface for B-splines.
// SourceFiles
//   b_spline_edge.cpp
#ifndef b_spline_edge_hpp_
#define b_spline_edge_hpp_
#include "curved_edge.hpp"
#include "b_spline.hpp"
namespace mousse
{
class BSplineEdge
:
  public curvedEdge,
  public BSpline
{
  // Private Member Functions
    //- Disallow default bitwise copy construct
    BSplineEdge(const BSplineEdge&);
    //- Disallow default bitwise assignment
    void operator=(const BSplineEdge&);
public:
  //- Runtime type information
  TypeName("BSpline");
  // Constructors
    //- Construct from components
    BSplineEdge
    (
      const pointField&,
      const label start,
      const label end,
      const pointField& internalPoints
    );
    //- Construct from Istream, setting pointsList
    BSplineEdge(const pointField&, Istream&);
  //- Destructor
  virtual ~BSplineEdge();
  // Member Functions
    //- Return the point position corresponding to the curve parameter
    //  0 <= lambda <= 1
    virtual point position(const scalar) const;
    //- Return the length of the spline curve (not implemented)
    virtual scalar length() const;
};
}  // namespace mousse
#endif
