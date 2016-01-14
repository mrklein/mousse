// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_parcel_injection_data.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(reactingParcelInjectionData, 0);
}
// Constructors 
mousse::reactingParcelInjectionData::reactingParcelInjectionData()
:
  thermoParcelInjectionData(),
  Y_()
{}
mousse::reactingParcelInjectionData::reactingParcelInjectionData
(
  const dictionary& dict
)
:
  thermoParcelInjectionData(dict),
  Y_(dict.lookup("Y"))
{}
// Destructor 
mousse::reactingParcelInjectionData::~reactingParcelInjectionData()
{}
