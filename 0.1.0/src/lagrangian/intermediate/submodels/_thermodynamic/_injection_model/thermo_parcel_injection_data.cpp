// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "thermo_parcel_injection_data.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(thermoParcelInjectionData, 0);
}
// Constructors 
mousse::thermoParcelInjectionData::thermoParcelInjectionData()
:
  kinematicParcelInjectionData(),
  T_(0.0),
  Cp_(0.0)
{}
mousse::thermoParcelInjectionData::thermoParcelInjectionData
(
  const dictionary& dict
)
:
  kinematicParcelInjectionData(dict),
  T_(readScalar(dict.lookup("T"))),
  Cp_(readScalar(dict.lookup("Cp")))
{}
// Destructor 
mousse::thermoParcelInjectionData::~thermoParcelInjectionData()
{}
