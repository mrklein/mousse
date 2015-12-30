// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_value.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace distributionModels
  {
    defineTypeNameAndDebug(fixedValue, 0);
    addToRunTimeSelectionTable(distributionModel, fixedValue, dictionary);
  }
}
// Constructors 
mousse::distributionModels::fixedValue::fixedValue
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel(typeName, dict, rndGen),
  value_(readScalar(distributionModelDict_.lookup("value")))
{}
mousse::distributionModels::fixedValue::fixedValue(const fixedValue& p)
:
  distributionModel(p),
  value_(p.value_)
{}
// Destructor 
mousse::distributionModels::fixedValue::~fixedValue()
{}
// Member Functions 
mousse::scalar mousse::distributionModels::fixedValue::fixedValue::sample() const
{
  return value_;
}
mousse::scalar mousse::distributionModels::fixedValue::fixedValue::minValue() const
{
  return value_;
}
mousse::scalar mousse::distributionModels::fixedValue::fixedValue::maxValue() const
{
  return value_;
}
mousse::scalar mousse::distributionModels::fixedValue::fixedValue::meanValue() const
{
  return value_;
}
