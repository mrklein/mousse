#ifndef CORE_MESHES_PRIMITIVE_SHAPES_OBJECT_HIT_HPP_
#define CORE_MESHES_PRIMITIVE_SHAPES_OBJECT_HIT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::objectHit
// Description
//   This class describes a combination of target object index and success flag.
#include "bool.hpp"
#include "label.hpp"
namespace mousse
{
class objectHit
{
  // Private data
    //- Hit success
    bool hit_;
    //- Object of hit
    label hitObject_;
public:
  // Constructors
    //- Construct null
    objectHit()
    :
      hit_(false),
      hitObject_(-1)
    {}
    //- Construct from components
    objectHit(const bool success, const label obj)
    :
      hit_(success),
      hitObject_(obj)
    {}
    //- Construct from Istream
    objectHit(Istream& is)
    :
      hit_(readBool(is)),
      hitObject_(readLabel(is))
    {}
  // Member Functions
    //- Is there a hit
    bool hit() const
    {
      return hit_;
    }
    //- Return hit object
    label hitObject() const
    {
      return hitObject_;
    }
  // Friend Operators
    inline friend bool operator==(const objectHit& a, const objectHit& b)
    {
      return (a.hit_ == b.hit_) && (a.hitObject_ == b.hitObject_);
    }
    inline friend bool operator!=(const objectHit& a, const objectHit& b)
    {
      return !(a == b);
    }
  // Ostream operator
    inline friend Ostream& operator<<(Ostream& os, const objectHit& obj)
    {
      return os << obj.hit() << token::SPACE << obj.hitObject();
    }
};
}  // namespace mousse
#endif
