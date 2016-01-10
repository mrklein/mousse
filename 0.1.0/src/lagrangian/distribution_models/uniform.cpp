// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uniform.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace distributionModels
  {
    DEFINE_TYPE_NAME_AND_DEBUG(uniform, 0);
    ADD_TO_RUN_TIME_SELECTION_TABLE(distributionModel, uniform, dictionary);
  }
}
// Constructors 
mousse::distributionModels::uniform::uniform
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel(typeName, dict, rndGen),
  minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
  maxValue_(readScalar(distributionModelDict_.lookup("maxValue")))
{
  check();
}
mousse::distributionModels::uniform::uniform(const uniform& p)
:
  distributionModel(p),
  minValue_(p.minValue_),
  maxValue_(p.maxValue_)
{}
// Destructor 
mousse::distributionModels::uniform::~uniform()
{}
// Member Functions 
mousse::scalar mousse::distributionModels::uniform::sample() const
{
  return rndGen_.position<scalar>(minValue_, maxValue_);
}
mousse::scalar mousse::distributionModels::uniform::minValue() const
{
  return minValue_;
}
mousse::scalar mousse::distributionModels::uniform::maxValue() const
{
  return maxValue_;
}
mousse::scalar mousse::distributionModels::uniform::meanValue() const
{
  return 0.5*(minValue_ + maxValue_);
}
