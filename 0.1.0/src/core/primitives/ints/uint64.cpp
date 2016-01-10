// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uint64.hpp"
const uint64_t mousse::pTraits<uint64_t>::zero = 0;
const uint64_t mousse::pTraits<uint64_t>::one = 1;
const uint64_t mousse::pTraits<uint64_t>::min = INT64_MIN;
const uint64_t mousse::pTraits<uint64_t>::max = INT64_MAX;
const uint64_t mousse::pTraits<uint64_t>::rootMin = pTraits<uint64_t>::min;
const uint64_t mousse::pTraits<uint64_t>::rootMax = pTraits<uint64_t>::max;
const char* mousse::pTraits<uint64_t>::componentNames[] = { "x" };
mousse::pTraits<uint64_t>::pTraits(const uint64_t& p)
:
  p_(p)
{}
mousse::pTraits<uint64_t>::pTraits(Istream& is)
{
  is >> p_;
}
