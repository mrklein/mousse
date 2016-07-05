// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "linear.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace blendingMethods {
DEFINE_TYPE_NAME_AND_DEBUG(linear, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  blendingMethod,
  linear,
  dictionary
);
}
}
// Constructors 
mousse::blendingMethods::linear::linear
(
  const dictionary& dict,
  const wordList& phaseNames
)
:
  blendingMethod{dict}
{
  FOR_ALL_CONST_ITER(wordList, phaseNames, iter)
  {
    const word nameFull
    {
      IOobject::groupName("maxFullyDispersedAlpha", *iter)
    };
    maxFullyDispersedAlpha_.insert
    (
      *iter,
      {
        nameFull,
        dimless,
        dict.lookup(nameFull)
      }
    );
    const word namePart
    {
      IOobject::groupName("maxPartlyDispersedAlpha", *iter)
    };
    maxPartlyDispersedAlpha_.insert
    (
      *iter,
      {
        namePart,
        dimless,
        dict.lookup(namePart)
      }
    );
    if (maxFullyDispersedAlpha_[*iter] > maxPartlyDispersedAlpha_[*iter])
    {
      FATAL_ERROR_IN
      (
        "mousse::blendingMethods::linear::linear"
        "("
          "const dictionary& dict,"
          "const wordList& phaseNames"
        ")"
      )
      << "The supplied fully dispersed volume fraction for "
      << *iter
      << " is greater than the partly dispersed value."
      << endl << exit(FatalError);
    }
  }
}
// Destructor 
mousse::blendingMethods::linear::~linear()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::linear::f1
(
  const phaseModel& phase1,
  const phaseModel& /*phase2*/
) const
{
  const dimensionedScalar maxFullAlpha{maxFullyDispersedAlpha_[phase1.name()]};
  const dimensionedScalar maxPartAlpha{maxPartlyDispersedAlpha_[phase1.name()]};
  return
    min
    (
      max
      (
        (phase1 - maxFullAlpha)/(maxPartAlpha - maxFullAlpha + SMALL),
        scalar(0.0)
      ),
      scalar(1.0)
    );
}
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::linear::f2
(
  const phaseModel& /*phase1*/,
  const phaseModel& phase2
) const
{
  const dimensionedScalar maxFullAlpha{maxFullyDispersedAlpha_[phase2.name()]};
  const dimensionedScalar maxPartAlpha{maxPartlyDispersedAlpha_[phase2.name()]};
  return
    min
    (
      max
      (
        (maxPartAlpha - phase2)/(maxPartAlpha - maxFullAlpha + SMALL),
        scalar(0.0)
      ),
      scalar(1.0)
    );
}
