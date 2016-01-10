// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "isstream.hpp"
#include "osstream.hpp"
void mousse::ISstream::print(Ostream& os) const
{
  os  << "ISstream: " << name().c_str() << ' ';
  IOstream::print(os);
  IOstream::print(os, is_.rdstate());
}
void mousse::OSstream::print(Ostream& os) const
{
  os  << "OSstream: " << name().c_str() << ' ';
  IOstream::print(os);
  IOstream::print(os, os_.rdstate());
}
