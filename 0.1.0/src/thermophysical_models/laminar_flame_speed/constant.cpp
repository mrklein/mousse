// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace laminarFlameSpeedModels
{
  defineTypeNameAndDebug(constant, 0);
  addToRunTimeSelectionTable
  (
    laminarFlameSpeed,
    constant,
    dictionary
  );
}
}
// Constructors 
mousse::laminarFlameSpeedModels::constant::constant
(
  const dictionary& dict,
  const psiuReactionThermo& ct
)
:
  laminarFlameSpeed(dict, ct),
  Su_(dict.lookup("Su"))
{}
// Destructor 
mousse::laminarFlameSpeedModels::constant::~constant()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField>
mousse::laminarFlameSpeedModels::constant::operator()() const
{
  return tmp<volScalarField>
  (
    new volScalarField
    (
      IOobject
      (
        "Su0",
        psiuReactionThermo_.T().time().timeName(),
        psiuReactionThermo_.T().db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      ),
      psiuReactionThermo_.T().mesh(),
      Su_
    )
  );
}
