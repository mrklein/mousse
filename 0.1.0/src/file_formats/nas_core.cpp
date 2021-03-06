// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nas_core.hpp"
#include "istring_stream.hpp"


// Constructors 
mousse::fileFormats::NASCore::NASCore()
{}


// Member Functions 
mousse::scalar mousse::fileFormats::NASCore::parseNASCoord
(
  const string& s
)
{
  size_t expSign = s.find_last_of("+-");
  if (expSign != string::npos && expSign > 0 && !isspace(s[expSign-1])) {
    scalar mantissa = readScalar(IStringStream(s.substr(0, expSign))());
    scalar exponent = readScalar(IStringStream(s.substr(expSign+1))());
    if (s[expSign] == '-') {
      exponent = -exponent;
    }
    return mantissa * pow(10, exponent);
  }
  return readScalar(IStringStream(s)());
}

