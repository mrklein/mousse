// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
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
      IOobject::groupName("minFullyContinuousAlpha", *iter)
    };
    minFullyContinuousAlpha_.insert
    (
      *iter,
      dimensionedScalar
      {
        nameFull,
        dimless,
        dict.lookup(nameFull)
      }
    );
    const word namePart
    {
      IOobject::groupName("minPartlyContinuousAlpha", *iter)
    };
    minPartlyContinuousAlpha_.insert
    (
      *iter,
      dimensionedScalar
      {
        namePart,
        dimless,
        dict.lookup(namePart)
      }
    );
    if
    (
      minFullyContinuousAlpha_[*iter]
     < minPartlyContinuousAlpha_[*iter]
    )
    {
      FATAL_ERROR_IN
      (
        "mousse::blendingMethods::linear::linear"
        "("
          "const dictionary& dict,"
          "const wordList& phaseNames"
        ")"
      )
      << "The supplied fully continuous volume fraction for "
      << *iter
      << " is less than the partly continuous value."
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
  const phaseModel& /*phase1*/,
  const phaseModel& phase2
) const
{
  const dimensionedScalar
    minFullAlpha(minFullyContinuousAlpha_[phase2.name()]);
  const dimensionedScalar
    minPartAlpha(minPartlyContinuousAlpha_[phase2.name()]);
  return
    min
    (
      max
      (
        (phase2 - minPartAlpha)/(minFullAlpha - minPartAlpha + SMALL),
        scalar(0)
      ),
      scalar(1)
    );
}
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::linear::f2
(
  const phaseModel& phase1,
  const phaseModel& /*phase2*/
) const
{
  const dimensionedScalar
    minFullAlpha(minFullyContinuousAlpha_[phase1.name()]);
  const dimensionedScalar
    minPartAlpha(minPartlyContinuousAlpha_[phase1.name()]);
  return
    min
    (
      max
      (
        (phase1 - minPartAlpha)/(minFullAlpha - minPartAlpha + SMALL),
        scalar(0)
      ),
      scalar(1)
    );
}
