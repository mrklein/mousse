// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "rosin_rammler.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
  namespace distributionModels
  {
    defineTypeNameAndDebug(RosinRammler, 0);
    addToRunTimeSelectionTable(distributionModel, RosinRammler, dictionary);
  }
}
// Constructors 
mousse::distributionModels::RosinRammler::RosinRammler
(
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModel(typeName, dict, rndGen),
  minValue_(readScalar(distributionModelDict_.lookup("minValue"))),
  maxValue_(readScalar(distributionModelDict_.lookup("maxValue"))),
  d_(readScalar(distributionModelDict_.lookup("d"))),
  n_(readScalar(distributionModelDict_.lookup("n")))
{
  check();
}
mousse::distributionModels::RosinRammler::RosinRammler(const RosinRammler& p)
:
  distributionModel(p),
  minValue_(p.minValue_),
  maxValue_(p.maxValue_),
  d_(p.d_),
  n_(p.n_)
{}
// Destructor 
mousse::distributionModels::RosinRammler::~RosinRammler()
{}
// Member Functions 
mousse::scalar mousse::distributionModels::RosinRammler::sample() const
{
  scalar K = 1.0 - exp(-pow((maxValue_ - minValue_)/d_, n_));
  scalar y = rndGen_.sample01<scalar>();
  scalar x = minValue_ + d_*::pow(-log(1.0 - y*K), 1.0/n_);
  return x;
}
mousse::scalar mousse::distributionModels::RosinRammler::minValue() const
{
  return minValue_;
}
mousse::scalar mousse::distributionModels::RosinRammler::maxValue() const
{
  return maxValue_;
}
mousse::scalar mousse::distributionModels::RosinRammler::meanValue() const
{
  return d_;
}
