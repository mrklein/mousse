// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "arc_edge.hpp"
#include "unit_conversion.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(arcEdge, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE(curvedEdge, arcEdge, Istream);
}
// Private Member Functions 
mousse::cylindricalCS mousse::arcEdge::calcAngle()
{
  vector a = p2_ - p1_;
  vector b = p3_ - p1_;
  // find centre of arcEdge
  scalar asqr = a & a;
  scalar bsqr = b & b;
  scalar adotb = a & b;
  scalar denom = asqr*bsqr - adotb*adotb;
  if (mag(denom) < VSMALL)
  {
    FATAL_ERROR_IN("cylindricalCS arcEdge::calcAngle()")
      << "Invalid arc definition - are the points co-linear?  Denom ="
      << denom
      << abort(FatalError);
  }
  scalar fact = 0.5*(bsqr - adotb)/denom;
  point centre = 0.5*a + fact*((a ^ b) ^ a);
  centre += p1_;
  // find position vectors w.r.t. the arcEdge centre
  vector r1(p1_ - centre);
  vector r2(p2_ - centre);
  vector r3(p3_ - centre);
  // find angles
  angle_ = radToDeg(acos((r3 & r1)/(mag(r3) * mag(r1))));
  // check if the vectors define an exterior or an interior arcEdge
  if (((r1 ^ r2) & (r1 ^ r3)) < 0.0)
  {
    angle_ = 360.0 - angle_;
  }
  vector tempAxis;
  if (angle_ <= 180.0)
  {
    tempAxis = r1 ^ r3;
    if (mag(tempAxis)/(mag(r1)*mag(r3)) < 0.001)
    {
      tempAxis = r1 ^ r2;
    }
  }
  else
  {
    tempAxis = r3 ^ r1;
  }
  radius_ = mag(r3);
  // set up and return the local coordinate system
  return cylindricalCS("arcEdgeCS", centre, tempAxis, r1);
}
// Constructors 
mousse::arcEdge::arcEdge
(
  const pointField& points,
  const label start,
  const label end,
  const point& pMid
)
:
  curvedEdge(points, start, end),
  p1_(points_[start_]),
  p2_(pMid),
  p3_(points_[end_]),
  cs_(calcAngle())
{}
mousse::arcEdge::arcEdge(const pointField& points, Istream& is)
:
  curvedEdge(points, is),
  p1_(points_[start_]),
  p2_(is),
  p3_(points_[end_]),
  cs_(calcAngle())
{}
// Member Functions 
mousse::point mousse::arcEdge::position(const scalar lambda) const
{
  if (lambda < -SMALL || lambda > 1 + SMALL)
  {
    FATAL_ERROR_IN("arcEdge::position(const scalar lambda) const")
      << "Parameter out of range, lambda = " << lambda
      << abort(FatalError);
  }
  if (lambda < SMALL)
  {
    return p1_;
  }
  else if (lambda > 1 - SMALL)
  {
    return p3_;
  }
  else
  {
    return cs_.globalPosition(vector(radius_, lambda*angle_, 0.0));
  }
}
mousse::scalar mousse::arcEdge::length() const
{
  return degToRad(angle_*radius_);
}
