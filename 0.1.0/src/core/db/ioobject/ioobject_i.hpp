// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mousse_version.hpp"
// Member Functions 
template<class Name>
inline mousse::word mousse::IOobject::groupName(Name name, const word& group)
{
  if (group != word::null)
  {
    return name + ('.' + group);
  }
  else
  {
    return name;
  }
}
template<class Stream>
inline Stream& mousse::IOobject::writeBanner(Stream& os, bool noHint)
{
  if (! noHint)
  {
    os  << "// -*- c++ -*-\n";
  }
  os << "// mousse: CFD toolbox (v. " << mousse_version << ")\n";
  return os;
}
template<class Stream>
inline Stream& mousse::IOobject::writeDivider(Stream& os)
{
  os  <<
    "// ---\n";
  return os;
}
template<class Stream>
inline Stream& mousse::IOobject::writeEndDivider(Stream& os)
{
  os  << "\n"
    "// vim: set ft=openfoam sw=4 ts=4 sts=4 et:\n";
  return os;
}
