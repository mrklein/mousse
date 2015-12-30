// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ac3d_surface_format_core.hpp"
#include "istring_stream.hpp"
// Private Member Functions 
template<class Type>
Type mousse::fileFormats::AC3DsurfaceFormatCore::parse(const string& s)
{
  IStringStream ss(s);
  Type t;
  ss >> t;
  return t;
}
