// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "ulabel.hpp"


#if WM_LABEL_SIZE == 32
const char* const mousse::pTraits<uint64_t>::typeName = "uint64";
const char* const mousse::pTraits<uint32_t>::typeName = "uLabel";
#elif WM_LABEL_SIZE == 64
const char* const mousse::pTraits<uint64_t>::typeName = "uLabel";
const char* const mousse::pTraits<uint32_t>::typeName = "uint32";
#endif

mousse::uLabel mousse::pow(uLabel a, uLabel b)
{
  uLabel ans = 1;
  for (uLabel i=0; i<b; i++) {
    ans *= a;
  }
#ifdef FULLDEBUG
  if (b < 0)
  {
    FATAL_ERROR_IN("pow(uLabel a, uLabel b)")
      << "negative value for b is not supported"
      << abort(FatalError);
  }
#endif
  return ans;
}


mousse::uLabel mousse::factorial(uLabel n)
{
  static uLabel factTable[13] =
  {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320,
    362880, 3628800, 39916800, 479001600
  };
#ifdef FULLDEBUG
  if (n > 12 && n < 0) {
    FATAL_ERROR_IN("factorial(uLabel n)")
      << "n value out of range"
      << abort(FatalError);
  }
#endif
  return factTable[n];
}
