// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cached_random.hpp"
#include "pstream.hpp"
// Member Functions 
template<class Type>
Type mousse::cachedRandom::sample01()
{
  Type value;
  for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
  {
    value.component(cmpt) = scalar01();
  }
  return value;
}
template<class Type>
Type mousse::cachedRandom::GaussNormal()
{
  Type value;
  for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
  {
    value.component(cmpt) = GaussNormal<scalar>();
  }
  return value;
}
template<class Type>
Type mousse::cachedRandom::position(const Type& start, const Type& end)
{
  Type value(start);
  for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
  {
    value.component(cmpt) +=
      scalar01()*(end.component(cmpt) - start.component(cmpt));
  }
  return value;
}
template<class Type>
void mousse::cachedRandom::randomise01(Type& value)
{
  value = sample01<Type>();
}
template<class Type>
Type mousse::cachedRandom::globalSample01()
{
  Type value = -GREAT*pTraits<Type>::one;
  if (Pstream::master())
  {
    value = sample01<Type>();
  }
  Pstream::scatter(value);
  return value;
}
template<class Type>
Type mousse::cachedRandom::globalGaussNormal()
{
  Type value = -GREAT*pTraits<Type>::one;
  if (Pstream::master())
  {
    value = GaussNormal<Type>();
  }
  Pstream::scatter(value);
  return value;
}
template<class Type>
Type mousse::cachedRandom::globalPosition(const Type& start, const Type& end)
{
  Type value = -GREAT*pTraits<Type>::one;
  if (Pstream::master())
  {
    value = position<Type>(start, end);
  }
  Pstream::scatter(value);
  return value;
}
template<class Type>
void mousse::cachedRandom::globalRandomise01(Type& value)
{
  value = -GREAT*pTraits<Type>::one;
  if (Pstream::master())
  {
    value = sample01<Type>();
  }
  Pstream::scatter(value);
}
