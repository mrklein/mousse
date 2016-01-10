// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "one.hpp"
namespace mousse
{
inline const one& operator*(const one& o, const one&)
{
  return o;
}
template<class Type>
inline const Type& operator*(const Type& t, const one&)
{
  return t;
}
template<class Type>
inline const Type& operator*(const one&, const Type& t)
{
  return t;
}
inline const one& operator/(const one& o, const one&)
{
  return o;
}
template<class Type>
inline Type operator/(const one&, const Type& t)
{
  return scalar(1)/t;
}
template<class Type>
inline const Type& operator/(const Type& t, const one&)
{
  return t;
}
}  // namespace mousse
