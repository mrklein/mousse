// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reacting_multiphase_parcel_injection_data.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(reactingMultiphaseParcelInjectionData, 0);

}


// Constructors 
mousse::reactingMultiphaseParcelInjectionData::
reactingMultiphaseParcelInjectionData()
:
  reactingParcelInjectionData{},
  YGas_{},
  YLiquid_{},
  YSolid_{}
{}


mousse::reactingMultiphaseParcelInjectionData::
reactingMultiphaseParcelInjectionData
(
  const dictionary& dict
)
:
  reactingParcelInjectionData{dict},
  YGas_{dict.lookup("YGas")},
  YLiquid_{dict.lookup("YLiquid")},
  YSolid_{dict.lookup("YSolid")}
{}


// Destructor 
mousse::reactingMultiphaseParcelInjectionData::
~reactingMultiphaseParcelInjectionData()
{}

