// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hyperbolic.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse {
namespace blendingMethods {
DEFINE_TYPE_NAME_AND_DEBUG(hyperbolic, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  blendingMethod,
  hyperbolic,
  dictionary
);
}
}
// Constructors 
mousse::blendingMethods::hyperbolic::hyperbolic
(
  const dictionary& dict,
  const wordList& phaseNames
)
:
  blendingMethod{dict},
  transitionAlphaScale_
  {
    "transitionAlphaScale",
    dimless,
    dict.lookup("transitionAlphaScale")
  }
{
  FOR_ALL_CONST_ITER(wordList, phaseNames, iter)
  {
    const word name{IOobject::groupName("maxDispersedAlpha", *iter)};
    maxDispersedAlpha_.insert
    (
      *iter,
      {
        name,
        dimless,
        dict.lookup(name)
      }
    );
  }
}
// Destructor 
mousse::blendingMethods::hyperbolic::~hyperbolic()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::hyperbolic::f1
(
  const phaseModel& phase1,
  const phaseModel& /*phase2*/
) const
{
  return
    (1 + tanh((4/transitionAlphaScale_)
              *(phase1 - maxDispersedAlpha_[phase1.name()])))/2;
}
mousse::tmp<mousse::volScalarField> mousse::blendingMethods::hyperbolic::f2
(
  const phaseModel& /*phase1*/,
  const phaseModel& phase2
) const
{
  return
    (1 + tanh((4/transitionAlphaScale_)
              *(maxDispersedAlpha_[phase2.name()] - phase2)))/2;
}
