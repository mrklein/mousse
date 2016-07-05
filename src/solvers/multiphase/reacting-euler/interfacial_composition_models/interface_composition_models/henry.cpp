// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "henry.hpp"
// Constructors 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>::Henry
(
  const dictionary& dict,
  const phasePair& pair
)
:
  InterfaceCompositionModel<Thermo, OtherThermo>{dict, pair},
  k_{dict.lookup("k")},
  YSolvent_
  {
    {
      IOobject::groupName("YSolvent", pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh()
    },
    pair.phase1().mesh(),
    {"one", dimless, 1}
  }
{
  if (k_.size() != this->speciesNames_.size())
  {
    FATAL_ERROR_IN
    (
      "template<class Thermo, class OtherThermo> "
      "mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>:: "
      "Henry "
      "( "
        "const dictionary& dict, "
        "const phasePair& pair "
      ")"
    )
    << "Differing number of species and solubilities"
    << exit(FatalError);
  }
}
// Destructor 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>::~Henry()
{}
// Member Functions 
template<class Thermo, class OtherThermo>
void mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>::update
(
  const volScalarField& Tf
)
{
  YSolvent_ = scalar(1);
  FOR_ALL_CONST_ITER(hashedWordList, this->speciesNames_, iter)
  {
    YSolvent_ -= Yf(*iter, Tf);
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>::Yf
(
  const word& speciesName,
  const volScalarField& /*Tf*/
) const
{
  if (this->speciesNames_.contains(speciesName))
  {
    const label index = this->speciesNames_[speciesName];
    return
      k_[index]*this->otherThermo_.composition().Y(speciesName)
      *this->otherThermo_.rhoThermo::rho()/this->thermo_.rhoThermo::rho();
  }
  else
  {
    return
      YSolvent_*this->thermo_.composition().Y(speciesName);
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Henry<Thermo, OtherThermo>::YfPrime
(
  const word& /*speciesName*/,
  const volScalarField& /*Tf*/
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        IOobject::groupName("YfPrime", this->pair_.name()),
        this->pair_.phase1().mesh().time().timeName(),
        this->pair_.phase1().mesh()
      },
      this->pair_.phase1().mesh(),
      {"zero", dimless/dimTemperature, 0}
    }
  };
}
