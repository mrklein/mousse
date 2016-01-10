// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "surface_registry.hpp"
#include "time.hpp"
// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(surfaceRegistry, 0);
}
const mousse::word mousse::surfaceRegistry::prefix("surfaces");
mousse::word mousse::surfaceRegistry::defaultName("default");
// Constructors
mousse::surfaceRegistry::surfaceRegistry
(
  const objectRegistry& obr,
  const word& surfName
)
:
  objectRegistry{
    IOobject{
      ( surfName.size() ? surfName : defaultName ),
      obr.time().timeName(),
      prefix,
      obr,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    }
  }
{}

// Destructor
mousse::surfaceRegistry::~surfaceRegistry()
{}
