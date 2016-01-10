// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cached_random.hpp"
#include "os_specific.hpp"
#include "pstream_reduce_ops.hpp"
// private Member Functions 
mousse::scalar mousse::cachedRandom::scalar01()
{
  if (sampleI_ < 0)
  {
    return osRandomDouble();
  }
  if (sampleI_ == samples_.size() - 1)
  {
    scalar s = samples_[sampleI_];
    sampleI_ = 0;
    return s;
  }
  else
  {
    scalar s = samples_[sampleI_];
    sampleI_++;
    return s;
  }
}
// Constructors 
mousse::cachedRandom::cachedRandom(const label seed, const label count)
:
  seed_(1),
  samples_(0),
  sampleI_(-1),
  hasGaussSample_(false),
  gaussSample_(0)
{
  if (seed > 1)
  {
    seed_ = seed;
  }
  // Samples will be cached if count > 0
  if (count > 0)
  {
    samples_.setSize(count);
    sampleI_ = 0;
  }
  // Initialise samples
  osRandomSeed(seed_);
  FOR_ALL(samples_, i)
  {
    samples_[i] = osRandomDouble();
  }
}
mousse::cachedRandom::cachedRandom(const cachedRandom& cr, const bool reset)
:
  seed_(cr.seed_),
  samples_(cr.samples_),
  sampleI_(cr.sampleI_),
  hasGaussSample_(cr.hasGaussSample_),
  gaussSample_(cr.gaussSample_)
{
  if (reset)
  {
    hasGaussSample_ = false;
    gaussSample_ = 0;
  }
  if (sampleI_ == -1)
  {
    WARNING_IN
    (
      "mousse::cachedRandom::cachedRandom(const cachedRandom& cr)"
    )   << "Copy constructor called, but samples not being cached. "
      << "This may lead to non-repeatable behaviour" << endl;
    osRandomSeed(seed_);
  }
  if (reset && samples_.size())
  {
    sampleI_ = 0;
  }
}
// Destructor 
mousse::cachedRandom::~cachedRandom()
{}
// Member Functions 
template<>
mousse::scalar mousse::cachedRandom::sample01()
{
  return scalar01();
}
template<>
mousse::label mousse::cachedRandom::sample01()
{
  return round(scalar01());
}
template<>
mousse::scalar mousse::cachedRandom::GaussNormal()
{
  if (hasGaussSample_)
  {
    hasGaussSample_ = false;
    return gaussSample_;
  }
  else
  {
    scalar rsq, v1, v2;
    do
    {
      v1 = 2*scalar01() - 1;
      v2 = 2*scalar01() - 1;
      rsq = sqr(v1) + sqr(v2);
    } while (rsq >= 1 || rsq == 0);
    scalar fac = sqrt(-2*log(rsq)/rsq);
    gaussSample_ = v1*fac;
    hasGaussSample_ = true;
    return v2*fac;
  }
}
template<>
mousse::label mousse::cachedRandom::GaussNormal()
{
  return round(GaussNormal<scalar>());
}
template<>
mousse::scalar mousse::cachedRandom::position
(
  const scalar& start,
  const scalar& end
)
{
  return start + scalar01()*(end - start);
}
template<>
mousse::label mousse::cachedRandom::position(const label& start, const label& end)
{
  return start + round(scalar01()*(end - start));
}
template<>
mousse::scalar mousse::cachedRandom::globalSample01()
{
  scalar value = -GREAT;
  if (Pstream::master())
  {
    value = scalar01();
  }
  Pstream::scatter(value);
  return value;
}
template<>
mousse::label mousse::cachedRandom::globalSample01()
{
  scalar value = -GREAT;
  if (Pstream::master())
  {
    value = scalar01();
  }
  Pstream::scatter(value);
  return round(value);
}
template<>
mousse::scalar mousse::cachedRandom::globalGaussNormal()
{
  scalar value = -GREAT;
  if (Pstream::master())
  {
    value = GaussNormal<scalar>();
  }
  Pstream::scatter(value);
  return value;
}
template<>
mousse::label mousse::cachedRandom::globalGaussNormal()
{
  scalar value = -GREAT;
  if (Pstream::master())
  {
    value = GaussNormal<scalar>();
  }
  Pstream::scatter(value);
  return round(value);
}
template<>
mousse::scalar mousse::cachedRandom::globalPosition
(
  const scalar& start,
  const scalar& end
)
{
  scalar value = -GREAT;
  if (Pstream::master())
  {
    value = scalar01()*(end - start);
  }
  Pstream::scatter(value);
  return start + value;
}
template<>
mousse::label mousse::cachedRandom::globalPosition
(
  const label& start,
  const label& end
)
{
  label value = labelMin;
  if (Pstream::master())
  {
    value = round(scalar01()*(end - start));
  }
  Pstream::scatter(value);
  return start + value;
}
