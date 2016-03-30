// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scalar.hpp"


namespace mousse {

scalar readScalar(Istream& is)
{
  scalar rs;
  is >> rs;
  return rs;
}

}  // namespace mousse
