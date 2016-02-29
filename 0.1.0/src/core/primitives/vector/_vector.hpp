#ifndef CORE_PRIMITIVES_VECTOR_TVECTOR_HPP_
#define CORE_PRIMITIVES_VECTOR_TVECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Vector
// Description
//   Templated 3D Vector derived from VectorSpace adding construction from
//   3 components, element access using x(), y() and z() member functions and
//   the inner-product (dot-product) and cross product operators.
//   A centre() member function which returns the Vector for which it is called
//   is defined so that point which is a typedef to Vector\<scalar\> behaves as
//   other shapes in the shape hierachy.
#include "vector_space.hpp"
namespace mousse
{
template<class T> class List;
template<class Cmpt>
class Vector
:
  public VectorSpace<Vector<Cmpt>, Cmpt, 3>
{
public:
  //- Equivalent type of labels used for valid component indexing
  typedef Vector<label> labelType;
  // Member constants
    enum
    {
      rank = 1 // Rank of Vector is 1
    };
  // Static data members
    static const char* const typeName;
    static const char* componentNames[];
    static const Vector zero;
    static const Vector one;
    static const Vector max;
    static const Vector min;
    static const Vector rootMax;
    static const Vector rootMin;
  //- Component labeling enumeration
  enum components { X, Y, Z };
  // Constructors
    //- Construct null
    inline Vector();
    //- Construct given VectorSpace of the same rank
    template<class Cmpt2>
    inline Vector(const VectorSpace<Vector<Cmpt2>, Cmpt2, 3>&);
    //- Construct given three components
    inline Vector(const Cmpt& vx, const Cmpt& vy, const Cmpt& vz);
    //- Construct from Istream
    inline Vector(Istream&);
  // Member Functions
    // Access
      inline const Cmpt& x() const;
      inline const Cmpt& y() const;
      inline const Cmpt& z() const;
      inline Cmpt& x();
      inline Cmpt& y();
      inline Cmpt& z();
    //- Return *this (used for point which is a typedef to Vector<scalar>.
    inline const Vector<Cmpt>& centre
    (
      const mousse::List<Vector<Cmpt> >&
    ) const;
};
template<class Cmpt>
class typeOfRank<Cmpt, 1>
{
public:
  typedef Vector<Cmpt> type;
};
template<class Cmpt>
class symmTypeOfRank<Cmpt, 1>
{
public:
  typedef Vector<Cmpt> type;
};
}  // namespace mousse

namespace mousse
{
// Constructors 
template<class Cmpt>
inline Vector<Cmpt>::Vector()
{}
template<class Cmpt>
template<class Cmpt2>
inline Vector<Cmpt>::Vector(const VectorSpace<Vector<Cmpt2>, Cmpt2, 3>& vs)
:
  VectorSpace<Vector<Cmpt>, Cmpt, 3>{vs}
{}
template<class Cmpt>
inline Vector<Cmpt>::Vector(const Cmpt& vx, const Cmpt& vy, const Cmpt& vz)
{
  this->v_[X] = vx;
  this->v_[Y] = vy;
  this->v_[Z] = vz;
}
template<class Cmpt>
inline Vector<Cmpt>::Vector(Istream& is)
:
  VectorSpace<Vector<Cmpt>, Cmpt, 3>{is}
{}
// Member Functions 
template<class Cmpt>
inline const Cmpt&  Vector<Cmpt>::x() const
{
  return this->v_[X];
}
template<class Cmpt>
inline const Cmpt&  Vector<Cmpt>::y() const
{
  return this->v_[Y];
}
template<class Cmpt>
inline const Cmpt&  Vector<Cmpt>::z() const
{
  return this->v_[Z];
}
template<class Cmpt>
inline Cmpt& Vector<Cmpt>::x()
{
  return this->v_[X];
}
template<class Cmpt>
inline Cmpt& Vector<Cmpt>::y()
{
  return this->v_[Y];
}
template<class Cmpt>
inline Cmpt& Vector<Cmpt>::z()
{
  return this->v_[Z];
}
// Member Functions 
template<class Cmpt>
inline const Vector<Cmpt>& Vector<Cmpt>::centre
(
  const mousse::List<Vector<Cmpt> >&
)const
{
  return *this;
}
// Global Operators 
template<class Cmpt>
inline typename innerProduct<Vector<Cmpt>, Vector<Cmpt> >::type
operator&(const Vector<Cmpt>& v1, const Vector<Cmpt>& v2)
{
  return Cmpt(v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
}
template<class Cmpt>
inline Vector<Cmpt> operator^(const Vector<Cmpt>& v1, const Vector<Cmpt>& v2)
{
  return {v1.y()*v2.z() - v1.z()*v2.y(),
          v1.z()*v2.x() - v1.x()*v2.z(),
          v1.x()*v2.y() - v1.y()*v2.x()};
}
}  // namespace mousse
#endif
