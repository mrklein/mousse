// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cached_random.hpp"
inline mousse::label mousse::cachedRandom::seed() const
{
  return seed_;
}
inline const mousse::scalarList& mousse::cachedRandom::samples() const
{
  return samples_;
}
inline mousse::label mousse::cachedRandom::sampleI() const
{
  return sampleI_;
}
inline mousse::label& mousse::cachedRandom::sampleI()
{
  return sampleI_;
}
