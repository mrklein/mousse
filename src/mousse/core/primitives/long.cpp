// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "long.hpp"

const long mousse::pTraits<long>::zero = 0;
const long mousse::pTraits<long>::one = 1;
const long mousse::pTraits<long>::min = LONG_MIN;
const long mousse::pTraits<long>::max = LONG_MAX;
const long mousse::pTraits<long>::rootMin = pTraits<long>::min;
const long mousse::pTraits<long>::rootMax = pTraits<long>::max;
const char* mousse::pTraits<long>::componentNames[] = { "x" };


mousse::pTraits<long>::pTraits(const long& p)
:
  p_{p}
{}


mousse::pTraits<long>::pTraits(Istream& is)
{
  is >> p_;
}
