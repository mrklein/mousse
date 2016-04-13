#ifndef CORE_PRIMITIVES_VECTOR_COMPLEX_VECTOR_HPP_
#define CORE_PRIMITIVES_VECTOR_COMPLEX_VECTOR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   complex_vector.cpp

#include "complex.hpp"
#include "vector.hpp"


namespace mousse {

typedef Vector<complex> complexVector;

// Friend Operators 
inline complexVector operator*(const complex& v1, const complexVector& v2)
{
  return {v1*v2.x(),
          v1*v2.y(),
          v1*v2.z()};
}


inline complexVector operator*(const complexVector& v2, const complex& v1)
{
  return {v1*v2.x(),
          v1*v2.y(),
          v1*v2.z()};
}


inline complexVector operator/(const complexVector& v1, const complex& v2)
{
  return {v1.x()/v2,
          v1.y()/v2,
          v1.z()/v2};
}


inline complexVector operator/(const complex& v1, const complexVector& v2)
{
  return {v1/v2.x(),
          v1/v2.y(),
          v1/v2.z()};
}


// complexVector dot product
inline complex operator&(const complexVector& v1, const complexVector& v2)
{
  return {v1.x()*v2.x().conjugate() + v1.y()*v2.y().conjugate()
    + v1.z()*v2.z().conjugate()};
}


// complexVector cross product
inline complexVector operator^(const complexVector& v1, const complexVector& v2)
{
  return {v1.y()*v2.z() - v1.z()*v2.y(),
          v1.z()*v2.x() - v1.x()*v2.z(),
          v1.x()*v2.y() - v1.y()*v2.x()};
}


// complexVector cross product
inline complexVector operator^(const vector& v1, const complexVector& v2)
{
  return {v1.y()*v2.z() - v1.z()*v2.y(),
          v1.z()*v2.x() - v1.x()*v2.z(),
          v1.x()*v2.y() - v1.y()*v2.x()};
}


}  // namespace mousse
#endif
