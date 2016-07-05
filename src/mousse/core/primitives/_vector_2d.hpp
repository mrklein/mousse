#ifndef CORE_PRIMITIVES_VECTOR_2D_TVECTOR_2D_HPP_
#define CORE_PRIMITIVES_VECTOR_2D_TVECTOR_2D_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Vector2D
// Description
//   Templated 2D Vector derived from VectorSpace adding construction from
//   2 components, element access using x() and y() member functions and
//   the inner-product (dot-product).

#include "vector_space.hpp"


namespace mousse {

template<class Cmpt>
class Vector2D
:
  public VectorSpace<Vector2D<Cmpt>, Cmpt, 2>
{
public:
  // Member constants
    enum
    {
      rank = 1 // Rank of Vector2D is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Vector2D zero;
    static const Vector2D one;
    static const Vector2D max;
    static const Vector2D min;
  //- Component labeling enumeration
  enum components { X, Y };
  // Constructors
    //- Construct null
    inline Vector2D();
    //- Construct given VectorSpace
    inline Vector2D(const VectorSpace<Vector2D<Cmpt>, Cmpt, 2>&);
    //- Construct given two components
    inline Vector2D(const Cmpt& vx, const Cmpt& vy);
    //- Construct from Istream
    inline Vector2D(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& x() const;
      inline const Cmpt& y() const;
      inline Cmpt& x();
      inline Cmpt& y();
    // Operators
      //- Perp dot product (dot product with perpendicular vector)
      inline scalar perp(const Vector2D<Cmpt>& b) const;
};

// Constructors 

// Construct null
template<class Cmpt>
inline Vector2D<Cmpt>::Vector2D()
{}


// Construct given VectorSpace
template<class Cmpt>
inline Vector2D<Cmpt>::Vector2D(const VectorSpace<Vector2D<Cmpt>, Cmpt, 2>& vs)
:
  VectorSpace<Vector2D<Cmpt>, Cmpt, 2>{vs}
{}


// Construct given three Cmpts
template<class Cmpt>
inline Vector2D<Cmpt>::Vector2D(const Cmpt& vx, const Cmpt& vy)
{
  this->v_[X] = vx;
  this->v_[Y] = vy;
}


// Construct from Istream
template<class Cmpt>
inline Vector2D<Cmpt>::Vector2D(Istream& is)
:
  VectorSpace<Vector2D<Cmpt>, Cmpt, 2>{is}
{}


// Member Functions 
template<class Cmpt>
inline const Cmpt&  Vector2D<Cmpt>::x() const
{
  return this->v_[X];
}


template<class Cmpt>
inline const Cmpt&  Vector2D<Cmpt>::y() const
{
  return this->v_[Y];
}


template<class Cmpt>
inline Cmpt& Vector2D<Cmpt>::x()
{
  return this->v_[X];
}


template<class Cmpt>
inline Cmpt& Vector2D<Cmpt>::y()
{
  return this->v_[Y];
}


// Global Operators 
template<class Cmpt>
inline typename innerProduct<Vector2D<Cmpt>, Vector2D<Cmpt> >::type
operator&(const Vector2D<Cmpt>& v1, const Vector2D<Cmpt>& v2)
{
  return Cmpt(v1.x()*v2.x() + v1.y()*v2.y());
}


template<class Cmpt>
inline scalar Vector2D<Cmpt>::perp(const Vector2D<Cmpt>& b) const
{
  return x()*b.y()-y()*b.x();
}


}  // namespace mousse
#endif
