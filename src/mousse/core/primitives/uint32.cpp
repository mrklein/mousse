// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uint32.hpp"


const uint32_t mousse::pTraits<uint32_t>::zero = 0;
const uint32_t mousse::pTraits<uint32_t>::one = 1;
const uint32_t mousse::pTraits<uint32_t>::min = INT32_MIN;
const uint32_t mousse::pTraits<uint32_t>::max = INT32_MAX;
const uint32_t mousse::pTraits<uint32_t>::rootMin = pTraits<uint32_t>::min;
const uint32_t mousse::pTraits<uint32_t>::rootMax = pTraits<uint32_t>::max;
const char* mousse::pTraits<uint32_t>::componentNames[] = { "x" };

mousse::pTraits<uint32_t>::pTraits(const uint32_t& p)
:
  p_{p}
{}


mousse::pTraits<uint32_t>::pTraits(Istream& is)
{
  is >> p_;
}
