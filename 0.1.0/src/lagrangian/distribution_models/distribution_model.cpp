// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distribution_model.hpp"
// Static Data Members
namespace mousse
{
  namespace distributionModels
  {
    defineTypeNameAndDebug(distributionModel, 0);
    defineRunTimeSelectionTable(distributionModel, dictionary);
  }
}
// Protected Member Functions 
void mousse::distributionModels::distributionModel::check() const
{
  if (minValue() < 0)
  {
    FatalErrorIn("distributionModels::distributionModel::check() const")
      << type() << "distribution: Minimum value must be greater than "
      << "zero." << nl << "Supplied minValue = " << minValue()
      << abort(FatalError);
  }
  if (maxValue() < minValue())
  {
    FatalErrorIn("distributionModels::distributionModel::check() const")
      << type() << "distribution: Maximum value is smaller than the "
      << "minimum value:" << nl << "    maxValue = " << maxValue()
      << ", minValue = " << minValue()
      << abort(FatalError);
  }
}
// Constructors 
mousse::distributionModels::distributionModel::distributionModel
(
  const word& name,
  const dictionary& dict,
  cachedRandom& rndGen
)
:
  distributionModelDict_(dict.subDict(name + "Distribution")),
  rndGen_(rndGen)
{}
mousse::distributionModels::distributionModel::distributionModel
(
  const distributionModel& p
)
:
  distributionModelDict_(p.distributionModelDict_),
  rndGen_(p.rndGen_)
{}
// Destructor 
mousse::distributionModels::distributionModel::~distributionModel()
{}
