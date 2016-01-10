// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "zero.hpp"
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
