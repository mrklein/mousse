// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multi_normal.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace distributionModels {

DEFINE_TYPE_NAME_AND_DEBUG(multiNormal, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(distributionModel, multiNormal, dictionary);

}
}


// Constructors 
mousse::distributionModels::multiNormal::multiNormal
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel{typeName, dict, rndGen},
  minValue_{readScalar(distributionModelDict_.lookup("minValue"))},
  maxValue_{readScalar(distributionModelDict_.lookup("maxValue"))},
  range_{maxValue_ - minValue_},
  expectation_{distributionModelDict_.lookup("expectation")},
  variance_{distributionModelDict_.lookup("variance")},
  strength_{distributionModelDict_.lookup("strength")}
{
  check();
  scalar sMax = 0;
  label n = strength_.size();
  for (label i=0; i<n; i++) {
    scalar x = expectation_[i];
    scalar s = strength_[i];
    for (label j=0; j<n; j++) {
      if (i == j)
        continue;
      scalar x2 = (x - expectation_[j])/variance_[j];
      scalar y = exp(-0.5*x2*x2);
      s += strength_[j]*y;
    }
    sMax = max(sMax, s);
  }
  for (label i=0; i<n; i++) {
    strength_[i] /= sMax;
  }
}


mousse::distributionModels::multiNormal::multiNormal(const multiNormal& p)
:
  distributionModel{p},
  minValue_{p.minValue_},
  maxValue_{p.maxValue_},
  range_{p.range_},
  expectation_{p.expectation_},
  variance_{p.variance_},
  strength_{p.strength_}
{}


// Destructor 
mousse::distributionModels::multiNormal::~multiNormal()
{}


// Member Functions 
mousse::scalar mousse::distributionModels::multiNormal::sample() const
{
  scalar y = 0;
  scalar x = 0;
  label n = expectation_.size();
  bool success = false;
  while (!success) {
    x = minValue_ + range_*rndGen_.sample01<scalar>();
    y = rndGen_.sample01<scalar>();
    scalar p = 0.0;
    for (label i=0; i<n; i++) {
      scalar nu = expectation_[i];
      scalar sigma = variance_[i];
      scalar s = strength_[i];
      scalar v = (x - nu)/sigma;
      p += s*exp(-0.5*v*v);
    }
    if (y<p) {
      success = true;
    }
  }
  return x;
}


mousse::scalar mousse::distributionModels::multiNormal::minValue() const
{
  return minValue_;
}


mousse::scalar mousse::distributionModels::multiNormal::maxValue() const
{
  return maxValue_;
}


mousse::scalar mousse::distributionModels::multiNormal::meanValue() const
{
  scalar mean = 0.0;
  FOR_ALL(strength_, i) {
    mean += strength_[i]*expectation_[i];
  }
  return mean;
}

