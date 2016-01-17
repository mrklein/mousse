// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef products_hpp_
#define products_hpp_
#include "ptraits.hpp"
namespace mousse
{
template<class Cmpt, int rank>
class typeOfRank
{};
template<class Cmpt, int rank>
class symmTypeOfRank
{};
template<class arg1, class arg2>
class typeOfSum
{
public:
  typedef arg1 type;
};
template<class arg1, class arg2>
class outerProduct
{
public:
  typedef typename typeOfRank
  <
    typename pTraits<arg1>::cmptType,
    int(pTraits<arg1>::rank) + int(pTraits<arg2>::rank)
  >::type type;
};
template<class arg1, class arg2>
class crossProduct
{
public:
  typedef typename typeOfRank
  <
    typename pTraits<arg2>::cmptType,
    int(pTraits<arg1>::rank) + int(pTraits<arg2>::rank) - 1
  >::type type;
};
template<class arg1, class arg2>
class innerProduct
{
public:
  typedef typename typeOfRank
  <
    typename pTraits<arg1>::cmptType,
    int(pTraits<arg1>::rank) + int(pTraits<arg2>::rank) - 2
  >::type type;
};
template<class arg1, class arg2>
class scalarProduct
{
public:
  typedef typename pTraits<arg1>::cmptType type;
};
template<class arg1, int arg2>
class powProduct
{
public:
  typedef typename symmTypeOfRank
  <
    typename pTraits<arg1>::cmptType,
    arg2*int(pTraits<arg1>::rank)
  >::type type;
};
}  // namespace mousse
#endif
