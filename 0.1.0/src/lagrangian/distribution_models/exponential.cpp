// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "exponential.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace distributionModels {

DEFINE_TYPE_NAME_AND_DEBUG(exponential, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(distributionModel, exponential, dictionary);

}
}


// Constructors 
mousse::distributionModels::exponential::exponential
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel{typeName, dict, rndGen},
  minValue_{readScalar(distributionModelDict_.lookup("minValue"))},
  maxValue_{readScalar(distributionModelDict_.lookup("maxValue"))},
  lambda_{readScalar(distributionModelDict_.lookup("lambda"))}
{
  check();
}


mousse::distributionModels::exponential::exponential(const exponential& p)
:
  distributionModel{p},
  minValue_{p.minValue_},
  maxValue_{p.maxValue_},
  lambda_{p.lambda_}
{}


// Destructor 
mousse::distributionModels::exponential::~exponential()
{}


// Member Functions 
mousse::scalar mousse::distributionModels::exponential::sample() const
{
  scalar y = rndGen_.sample01<scalar>();
  scalar K = exp(-lambda_*maxValue_) - exp(-lambda_*minValue_);
  return -log(exp(-lambda_*minValue_) + y*K)/lambda_;
}


mousse::scalar mousse::distributionModels::exponential::minValue() const
{
  return minValue_;
}


mousse::scalar mousse::distributionModels::exponential::maxValue() const
{
  return maxValue_;
}


mousse::scalar mousse::distributionModels::exponential::meanValue() const
{
  return 1.0/lambda_;
}

