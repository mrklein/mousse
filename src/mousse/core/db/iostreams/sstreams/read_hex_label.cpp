// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_hex_label.hpp"
#include "read_hex.hpp"


mousse::label mousse::readHexLabel(ISstream& is)
{
  return ReadHex<label>(is);
}
