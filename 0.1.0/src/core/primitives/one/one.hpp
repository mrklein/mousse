#ifndef CORE_PRIMITIVES_ONE_ONE_HPP_
#define CORE_PRIMITIVES_ONE_ONE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::one
// Description
//   A class representing the concept of 1 (scalar(1.0)) used to avoid
//   unnecessary manipulations for objects which are known to be one at
//   compile-time.
namespace mousse
{
class one
{
public:
  // Constructors
    //- Construct null
    one()
    {}
};
}  // namespace mousse

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
  return scalar{1.0}/t;
}
template<class Type>
inline const Type& operator/(const Type& t, const one&)
{
  return t;
}
}  // namespace mousse
#endif
