// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "normal.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "mathematical_constants.hpp"
// Static Data Members
namespace mousse
{
  namespace distributionModels
  {
    defineTypeNameAndDebug(normal, 0);
    addToRunTimeSelectionTable(distributionModel, normal, dictionary);
  }
}
// Constructors 
mousse::distributionModels::normal::normal
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel(typeName, dict, rndGen),
  minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
  maxValue_(readScalar(distributionModelDict_.lookup("maxValue"))),
  expectation_(readScalar(distributionModelDict_.lookup("expectation"))),
  variance_(readScalar(distributionModelDict_.lookup("variance"))),
  a_(0.147)
{
  if (minValue_ < 0)
  {
    FatalErrorIn("normal::normal(const dictionary&, Random&)")
      << "Minimum value must be greater than zero. "
      << "Supplied minValue = " << minValue_
      << abort(FatalError);
  }
  if (maxValue_ < minValue_)
  {
    FatalErrorIn("normal::normal(const dictionary&, Random&)")
      << "Maximum value is smaller than the minimum value:"
      << "    maxValue = " << maxValue_ << ", minValue = " << minValue_
      << abort(FatalError);
  }
}
mousse::distributionModels::normal::normal(const normal& p)
:
  distributionModel(p),
  minValue_(p.minValue_),
  maxValue_(p.maxValue_),
  expectation_(p.expectation_),
  variance_(p.variance_),
  a_(p.a_)
{}
// Destructor 
mousse::distributionModels::normal::~normal()
{}
// Member Functions 
mousse::scalar mousse::distributionModels::normal::sample() const
{
  scalar a = erf((minValue_ - expectation_)/variance_);
  scalar b = erf((maxValue_ - expectation_)/variance_);
  scalar y = rndGen_.sample01<scalar>();
  scalar x = erfInv(y*(b - a) + a)*variance_ + expectation_;
  // Note: numerical approximation of the inverse function yields slight
  //       inaccuracies
  x = min(max(x, minValue_), maxValue_);
  return x;
}
mousse::scalar mousse::distributionModels::normal::minValue() const
{
  return minValue_;
}
mousse::scalar mousse::distributionModels::normal::maxValue() const
{
  return maxValue_;
}
mousse::scalar mousse::distributionModels::normal::meanValue() const
{
  return expectation_;
}
mousse::scalar mousse::distributionModels::normal::erfInv(const scalar y) const
{
  scalar k = 2.0/(constant::mathematical::pi*a_) +  0.5*log(1.0 - y*y);
  scalar h = log(1.0 - y*y)/a_;
  scalar x = sqrt(-k + sqrt(k*k - h));
  if (y < 0.0)
  {
    x *= -1.0;
  }
  return x;
}
