// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "label.hpp"

#if WM_LABEL_SIZE == 32
const char* const mousse::pTraits<int64_t>::typeName = "int64";
const char* const mousse::pTraits<int32_t>::typeName = "label";
#elif WM_LABEL_SIZE == 64
const char* const mousse::pTraits<int64_t>::typeName = "label";
const char* const mousse::pTraits<int32_t>::typeName = "int32";
#endif

mousse::label mousse::pow(label a, label b)
{
  label ans = 1;
  for (label i=0; i<b; i++)
  {
    ans *= a;
  }
#ifdef FULLDEBUG
  if (b < 0)
  {
    FATAL_ERROR_IN("pow(label a, label b)")
      << "negative value for b is not supported"
      << abort(FatalError);
  }
#endif
  return ans;
}


mousse::label mousse::factorial(label n)
{
  static label factTable[13] =
  {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320,
    362880, 3628800, 39916800, 479001600
  };
#ifdef FULLDEBUG
  if (n > 12 && n < 0)
  {
    FATAL_ERROR_IN("factorial(label n)")
      << "n value out of range"
      << abort(FatalError);
  }
#endif

  return factTable[n];
}
