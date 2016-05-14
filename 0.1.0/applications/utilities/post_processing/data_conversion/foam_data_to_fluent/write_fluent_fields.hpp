#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_DATA_TO_FLUENT_WRITE_FLUENT_FIELDS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_DATA_TO_FLUENT_WRITE_FLUENT_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project


#include "vol_fields.hpp"
#include "ostream.hpp"


namespace mousse {

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

