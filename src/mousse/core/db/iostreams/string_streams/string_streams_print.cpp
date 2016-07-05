// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "istring_stream.hpp"
#include "ostring_stream.hpp"


void mousse::IStringStream::print(Ostream& os) const
{
  os << "IStringStream " << name() << " : "
    << "buffer = \n" << str() << mousse::endl;
  ISstream::print(os);
}


void mousse::OStringStream::print(Ostream& os) const
{
  os << "OStringStream " << name() << " : "
    << "buffer = \n" << str() << mousse::endl;
  OSstream::print(os);
}
