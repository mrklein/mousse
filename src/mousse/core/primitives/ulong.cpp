// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ulong.hpp"


const unsigned long mousse::pTraits<unsigned long>::zero = 0;
const unsigned long mousse::pTraits<unsigned long>::one = 1;
const unsigned long mousse::pTraits<unsigned long>::min = ULONG_MIN;
const unsigned long mousse::pTraits<unsigned long>::max = ULONG_MAX;
const unsigned long mousse::pTraits<unsigned long>::rootMin =
  pTraits<unsigned long>::min;
const unsigned long mousse::pTraits<unsigned long>::rootMax =
  pTraits<unsigned long>::max;
const char* mousse::pTraits<unsigned long>::componentNames[] = { "x" };


mousse::pTraits<unsigned long>::pTraits(const unsigned long& p)
:
  p_{p}
{}


mousse::pTraits<unsigned long>::pTraits(Istream& is)
{
  is >> p_;
}
