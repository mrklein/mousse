// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "random.hpp"
#include "os_specific.hpp"
#if INT_MAX    != 2147483647
#    error "INT_MAX    != 2147483647"
#    error "The random number generator may not work!"
#endif


// Member Functions 
mousse::Random::Random(const label seed)
{
  if (seed > 1) {
    Seed = seed;
  } else {
    Seed = 1;
  }
  osRandomSeed(Seed);
}


int mousse::Random::bit()
{
  if (osRandomInteger() > INT_MAX/2) {
    return 1;
  } else {
    return 0;
  }
}


mousse::scalar mousse::Random::scalar01()
{
  return osRandomDouble();
}


mousse::vector mousse::Random::vector01()
{
  vector rndVec;
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
    rndVec.component(cmpt) = scalar01();
  }
  return rndVec;
}


mousse::sphericalTensor mousse::Random::sphericalTensor01()
{
  sphericalTensor rndTen;
  rndTen.ii() = scalar01();
  return rndTen;
}


mousse::symmTensor mousse::Random::symmTensor01()
{
  symmTensor rndTen;
  for (direction cmpt=0; cmpt<symmTensor::nComponents; cmpt++) {
    rndTen.component(cmpt) = scalar01();
  }
  return rndTen;
}


mousse::tensor mousse::Random::tensor01()
{
  tensor rndTen;
  for (direction cmpt=0; cmpt<tensor::nComponents; cmpt++) {
    rndTen.component(cmpt) = scalar01();
  }
  return rndTen;
}


mousse::label mousse::Random::integer(const label lower, const label upper)
{
  return lower + (osRandomInteger() % (upper+1-lower));
}


mousse::vector mousse::Random::position(const vector& start, const vector& end)
{
  vector rndVec(start);
  for (direction cmpt=0; cmpt<vector::nComponents; cmpt++) {
    rndVec.component(cmpt) +=
      scalar01()*(end.component(cmpt) - start.component(cmpt));
  }
  return rndVec;
}


void mousse::Random::randomise(scalar& s)
{
  s = scalar01();
}


void mousse::Random::randomise(vector& v)
{
  v = vector01();
}


void mousse::Random::randomise(sphericalTensor& st)
{
  st = sphericalTensor01();
}


void mousse::Random::randomise(symmTensor& st)
{
  st = symmTensor01();
}


void mousse::Random::randomise(tensor& t)
{
  t = tensor01();
}


mousse::scalar mousse::Random::GaussNormal()
{
  static int iset = 0;
  static scalar gset;
  scalar fac, rsq, v1, v2;
  if (iset == 0) {
    do {
      v1 = 2.0*scalar01() - 1.0;
      v2 = 2.0*scalar01() - 1.0;
      rsq = sqr(v1) + sqr(v2);
    } while (rsq >= 1.0 || rsq == 0.0);
    fac = sqrt(-2.0*log(rsq)/rsq);
    gset = v1*fac;
    iset = 1;
    return v2*fac;
  } else {
    iset = 0;
    return gset;
  }
}

