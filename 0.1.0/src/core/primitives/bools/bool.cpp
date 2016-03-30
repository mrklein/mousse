// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bool.hpp"


const char* const mousse::pTraits<bool>::typeName = "bool";
const bool mousse::pTraits<bool>::zero = false;
const bool mousse::pTraits<bool>::one = true;
const char* mousse::pTraits<bool>::componentNames[] = { "x" };


mousse::pTraits<bool>::pTraits(const bool& p)
:
  p_{p}
{}


mousse::pTraits<bool>::pTraits(Istream& is)
{
  is >> p_;
}
