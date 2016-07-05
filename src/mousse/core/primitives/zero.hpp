#ifndef CORE_PRIMITIVES_ZERO_ZERO_HPP_
#define CORE_PRIMITIVES_ZERO_ZERO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::zero
// Description
//   A class representing the concept of 0 used to avoid unnecessary
//   manipulations for objects that are known to be zero at compile-time.
namespace mousse
{
class zero
{
public:
  zero()
  {}
};
}  // namespace mousse

namespace mousse
{
template<class Type>
inline const Type& operator+(const Type& t, const zero&)
{
  return t;
}
template<class Type>
inline const Type& operator+(const zero&, const Type& t)
{
  return t;
}
template<class Type>
inline const Type& operator-(const Type& t, const zero&)
{
  return t;
}
template<class Type>
inline Type operator-(const zero&, const Type& t)
{
  return -t;
}
template<class Type>
inline zero operator*(const Type&, const zero&)
{
  return zero();
}
template<class Type>
inline zero operator*(const zero&, const Type&)
{
  return zero();
}
template<class Type>
inline zero operator/(const zero&, const Type&)
{
  return zero();
}
}  // namespace mousse
#endif
