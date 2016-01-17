// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uint.hpp"
#include "iostreams.hpp"
// IOstream Operators 
unsigned int mousse::readUint(Istream& is)
{
  unsigned int val;
  is >> val;
  return val;
}
