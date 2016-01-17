// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solution.hpp"
template<class FieldType>
void mousse::solution::cachePrintMessage
(
  const char* message,
  const word& name,
  const FieldType& vf
)
{
  if (solution::debug)
  {
    Info<< "Cache: " << message << token::SPACE << name
      << ", originating from " << vf.name()
      << " event No. " << vf.eventNo()
      << endl;
  }
}
