// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kinematic_parcel_injection_data.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(kinematicParcelInjectionData, 0);
}
// Constructors 
mousse::kinematicParcelInjectionData::kinematicParcelInjectionData()
:
  x_(point::zero),
  U_(vector::zero),
  d_(0.0),
  rho_(0.0),
  mDot_(0.0)
{}
mousse::kinematicParcelInjectionData::kinematicParcelInjectionData
(
  const dictionary& dict
)
:
  x_(dict.lookup("x")),
  U_(dict.lookup("U")),
  d_(readScalar(dict.lookup("d"))),
  rho_(readScalar(dict.lookup("rho"))),
  mDot_(readScalar(dict.lookup("mDot")))
{}
// Destructor 
mousse::kinematicParcelInjectionData::~kinematicParcelInjectionData()
{}
