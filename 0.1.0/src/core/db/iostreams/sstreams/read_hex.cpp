// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_hex.hpp"
#include <cctype>

template<class T>
T mousse::ReadHex(ISstream& is)
{
  // Takes into account that 'a' (or 'A') is 10
  static const int alphaOffset = toupper('A') - 10;
  // Takes into account that '0' is 0
  static const int zeroOffset = int('0');
  char c = 0;
  // Get next non-whitespace character
  while (is.get(c) && isspace(c))
  {}
  T result = 0;
  do
  {
    if (isspace(c) || c == 0) break;
    if (!isxdigit(c))
    {
      FATAL_IO_ERROR_IN("ReadHex(ISstream&)", is)
        << "Illegal hex digit: '" << c << "'"
        << exit(FatalIOError);
    }
    result <<= 4;
    if (isdigit(c))
    {
      result += int(c) - zeroOffset;
    }
    else
    {
      result += toupper(c) - alphaOffset;
    }
  } while (is.get(c));
  return result;
}
