// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef write_fluent_fields_hpp_
#define write_fluent_fields_hpp_

#include "vol_fields.hpp"
#include "ostream.hpp"

namespace mousse
{
void writeFluentField
(
  const volScalarField& phi,
  const label fluentFieldIdentifier,
  Ostream& stream
);
void writeFluentField
(
  const volVectorField& phi,
  const label fluentFieldIdentifier,
  Ostream& stream
);
}  // namespace mousse
#endif
