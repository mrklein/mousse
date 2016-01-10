// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PointIndexHit
// Description
//   This class describes the interaction of (usually) a face and a point.
//   It carries the info of a successful hit and (if successful),
//   returns the interaction point.
//   like pointHit but carries face (or cell, edge etc.) index
// SourceFiles

#ifndef _point_index_hit_hpp_
#define _point_index_hit_hpp_

#include "bool.hpp"
#include "point.hpp"
#include "token.hpp"
#include "istream.hpp"

namespace mousse
{
template<class Point>
class PointIndexHit
{
  // Private data
    //- Hit success
    bool hit_;
    //- Point of hit; invalid for misses
    Point hitPoint_;
    //- Label of face hit
    label index_;
public:
  // Constructors
    //- Construct from components
    PointIndexHit(const bool success, const Point& p, const label index)
    :
      hit_(success),
      hitPoint_(p),
      index_(index)
    {}
    //- Construct from point. Hit and distance set later
    PointIndexHit(const Point& p)
    :
      hit_(false),
      hitPoint_(p),
      index_(-1)
    {}
    //- Construct null
    PointIndexHit()
    :
      hit_(false),
      hitPoint_(vector::zero),
      index_(-1)
    {}
    //- Construct from Istream
    PointIndexHit(Istream& is)
    {
      is >> *this;
    }
  // Member Functions
    //- Is there a hit
    bool hit() const
    {
      return hit_;
    }
    //- Return index
    label index() const
    {
      return index_;
    }
    //- Return hit point
    const Point& hitPoint() const
    {
      if (!hit_)
      {
        FATAL_ERROR_IN("PointIndexHit::hitPoint() const")
          << "requested a hit point for a miss"
          << abort(FatalError);
      }
      return hitPoint_;
    }
    //- Return miss point
    const Point& missPoint() const
    {
      if (hit_)
      {
        FATAL_ERROR_IN("PointIndexHit::missPoint() const")
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
    Point& rawPoint()
    {
      return hitPoint_;
    }
    void setHit()
    {
      hit_ = true;
    }
    void setMiss()
    {
      hit_ = false;
    }
    void setPoint(const Point& p)
    {
      hitPoint_ = p;
    }
    void setIndex(const label index)
    {
      index_ = index;
    }
    bool operator==(const PointIndexHit& rhs) const
    {
      return
        hit_ == rhs.hit()
      && hitPoint_ == rhs.rawPoint()
      && index_ == rhs.index();
    }
    bool operator!=(const PointIndexHit& rhs) const
    {
      return !operator==(rhs);
    }
    void write(Ostream& os)
    {
      if (hit())
      {
        os << "hit:" << hitPoint() << " index:" << index();
      }
      else
      {
        os << "miss:" << missPoint() << " index:" << index();
      }
    }
    friend Ostream& operator<< (Ostream& os, const PointIndexHit& pHit)
    {
      if (os.format() == IOstream::ASCII)
      {
        os  << pHit.hit_ << token::SPACE << pHit.hitPoint_
          << token::SPACE << pHit.index_;
      }
      else
      {
        os.write
        (
          reinterpret_cast<const char*>(&pHit),
          sizeof(PointIndexHit)
        );
      }
      // Check state of Ostream
      os.check("Ostream& operator<<(Ostream&, const PointIndexHit&)");
      return os;
    }
    friend Istream& operator>>(Istream& is, PointIndexHit& pHit)
    {
      if (is.format() == IOstream::ASCII)
      {
        return is >> pHit.hit_ >> pHit.hitPoint_ >> pHit.index_;
      }
      else
      {
        is.read
        (
          reinterpret_cast<char*>(&pHit),
          sizeof(PointIndexHit)
        );
      }
      // Check state of Istream
      is.check("Istream& operator>>(Istream&, PointIndexHit&)");
      return is;
    }
};
}  // namespace mousse

#endif
