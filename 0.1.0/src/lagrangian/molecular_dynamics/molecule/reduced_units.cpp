// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reduced_units.hpp"
// Static Data Members
const  mousse::scalar mousse::reducedUnits::kb = 1.3806504e-23;
// Private Member Functions 
void mousse::reducedUnits::calcRefValues()
{
  if
  (
    refTime_ < VSMALL
  || refLength_ < VSMALL
  || refMass_ < VSMALL
  )
  {
    FATAL_ERROR_IN("mousse::reducedUnits::calcRefValues() ")
      << "One of more referencence values too small for floating point "
      << "calculation: "
      << "refTime_ = " << refTime_
      << ", refLength = " << refTemp_
      << ", refMass = " << refMass_
      << nl << abort(FatalError);
  }
  refEnergy_ = refLength_*refLength_*refMass_/(refTime_*refTime_);
  refTemp_ = refEnergy_ / kb;
  refForce_ = refEnergy_/refLength_;
  refVelocity_ = mousse::sqrt(refEnergy_/refMass_);
  refVolume_ = mousse::pow(refLength_,3.0);
  refPressure_ = refEnergy_/refVolume_;
  refMassDensity_ = refMass_/refVolume_;
  refNumberDensity_ = 1.0/refVolume_;
}
// Constructors 
mousse::reducedUnits::reducedUnits()
:
  refLength_{1e-9},
  refTime_{1e-12},
  refMass_{1.660538782e-27}
{
  calcRefValues();
}
mousse::reducedUnits::reducedUnits
(
  scalar refLength,
  scalar refTime,
  scalar refMass
)
:
  refLength_{refLength},
  refTime_{refTime},
  refMass_{refMass}
{
  calcRefValues();
}
mousse::reducedUnits::reducedUnits(const IOdictionary& reducedUnitsDict)
:
  refLength_{},
  refTime_{},
  refMass_{}
{
  setRefValues(reducedUnitsDict);
}
// Destructor 
mousse::reducedUnits::~reducedUnits()
{}
// Member Functions 
void mousse::reducedUnits::setRefValues
(
  scalar refLength,
  scalar refTime,
  scalar refMass
)
{
  refLength_ = refLength;
  refTime_ = refTime;
  refMass_ = refMass;
  calcRefValues();
}
void mousse::reducedUnits::setRefValues
(
  const IOdictionary& reducedUnitsDict
)
{
  refLength_ = readScalar(reducedUnitsDict.lookup("refLength"));
  refTime_ = readScalar(reducedUnitsDict.lookup("refTime"));
  refMass_  = readScalar(reducedUnitsDict.lookup("refMass"));
  calcRefValues();
}

