// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ioobject.hpp"
#include "token.hpp"
template<>
mousse::Ostream& mousse::operator<<(Ostream& os, const InfoProxy<IOobject>& ip)
{
  const IOobject& io = ip.t_;
  os  << "IOobject: "
    << io.type() << token::SPACE
    << io.name() << token::SPACE
    << io.path() << endl;
  return os;
}
