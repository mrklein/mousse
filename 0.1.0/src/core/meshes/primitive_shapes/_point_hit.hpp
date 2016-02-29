#ifndef CORE_MESHES_PRIMITIVE_SHAPES_TPOINT_HIT_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_TPOINT_HIT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PointHit
// Description
//   This class describes the interaction of a face and a point. It
//   carries the info of a successful hit and (if successful), returns
//   the interaction point.

#include "token.hpp"
#include "bool.hpp"

namespace mousse
{
// Forward declaration of classes
class Ostream;
// Forward declaration of friend functions and operators
template<class Point> class PointHit;
template<class Point>
inline Ostream& operator<<(Ostream&, const PointHit<Point>&);
template<class Point>
class PointHit
{
  // Private data
    //- Hit success
    bool hit_;
    //- Point of hit; for miss holds best estimate outside the object
    Point hitPoint_;
    //- Distance to hit point
    scalar distance_;
    //- Eligible miss
    bool eligibleMiss_;
public:
  // Constructors
    //- Construct null
    PointHit()
    :
      hit_(false),
      hitPoint_(vector::zero),
      distance_(GREAT),
      eligibleMiss_(false)
    {}
    //- Construct from components
    PointHit
    (
      const bool hit,
      const Point& p,
      const scalar dist,
      const bool eligibleMiss
    )
    :
      hit_(hit),
      hitPoint_(p),
      distance_(dist),
      eligibleMiss_(eligibleMiss)
    {}
    //- Construct from point. Hit and distance set later
    PointHit(const Point& p)
    :
      hit_(false),
      hitPoint_(p),
      distance_(GREAT),
      eligibleMiss_(false)
    {}
  // Member Functions
    //- Is there a hit
    bool hit() const
    {
      return hit_;
    }
    //- Return hit point
    const Point& hitPoint() const
    {
      if (!hit_)
      {
        FATAL_ERROR_IN("const Point& PointHit::hitPoint() const")
          << "requested a hit point for a miss"
          << abort(FatalError);
      }
      return hitPoint_;
    }
    //- Return distance to hit
    scalar distance() const
    {
      return distance_;
    }
    //- Return miss point
    const Point& missPoint() const
    {
      if (hit_)
      {
        FATAL_ERROR_IN("const Point& PointHit::missPoint() const")
          << "requested a miss point for a hit"
          << abort(FatalError);
      }
      return hitPoint_;
    }
    //- Return point with no checking
    const Point& rawPoint() const
    {
      return hitPoint_;
    }
    //- Is this an eligible miss
    bool eligibleMiss() const
    {
      return eligibleMiss_;
    }
    void setHit()
    {
      hit_ = true;
      eligibleMiss_ = false;
    }
    void setMiss(const bool eligible)
    {
      hit_ = false;
      eligibleMiss_ = eligible;
    }
    void setPoint(const Point& p)
    {
      hitPoint_ = p;
    }
    void setDistance(const scalar d)
    {
      distance_ = d;
    }
  // Ostream operator
    friend Ostream& operator<< <Point>
    (
      Ostream& os,
      const PointHit<Point>& b
    );
};
template<class Point>
inline Ostream& operator<<(Ostream& os, const PointHit<Point>& b)
{
  os<< b.hit() << token::SPACE
    << b.rawPoint() << token::SPACE
    << b.distance() << token::SPACE
    << b.eligibleMiss();
  return os;
}
}  // namespace mousse
#endif
