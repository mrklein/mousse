// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "saturated.hpp"
// Private Member Functions 
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::
wRatioByP() const
{
  return this->thermo_.composition().W(saturatedIndex_)
    /this->thermo_.composition().W()/this->thermo_.p();
}
// Constructors 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::Saturated
(
  const dictionary& dict,
  const phasePair& pair
)
:
  InterfaceCompositionModel<Thermo, OtherThermo>{dict, pair},
  saturatedName_{this->speciesNames_[0]},
  saturatedIndex_{this->thermo_.composition().species()[saturatedName_]},
  saturationModel_
  {
    saturationModel::New
    (
      dict.subDict("saturationPressure")
    )
  }
{
  if (this->speciesNames_.size() != 1)
  {
    FATAL_ERROR_IN
    (
      "template<class Thermo, class OtherThermo>"
      "mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::"
      "Saturated"
      "( "
        "const dictionary& dict, "
        "const phasePair& pair "
      ")"
    )
    << "Saturated model is suitable for one species only."
    << exit(FatalError);
  }
}
// Destructor 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::~Saturated()
{}
// Member Functions 
template<class Thermo, class OtherThermo>
void
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::update
(
  const volScalarField& /*Tf*/
)
{
  // do nothing
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::Yf
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (saturatedName_ == speciesName)
  {
    return wRatioByP()*saturationModel_->pSat(Tf);
  }
  else
  {
    const label speciesIndex
    {
      this->thermo_.composition().species()[speciesName]
    };
    return this->thermo_.Y()[speciesIndex]
      *(scalar(1) - wRatioByP()*saturationModel_->pSat(Tf))
      /max(scalar(1) - this->thermo_.Y()[saturatedIndex_], SMALL);
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Saturated<Thermo, OtherThermo>::YfPrime
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (saturatedName_ == speciesName)
  {
    return wRatioByP()*saturationModel_->pSatPrime(Tf);
  }
  else
  {
    const label speciesIndex
    (
      this->thermo_.composition().species()[speciesName]
    );
    return -this->thermo_.Y()[speciesIndex]
      *wRatioByP()*saturationModel_->pSatPrime(Tf)
      /max(scalar(1) - this->thermo_.Y()[saturatedIndex_], SMALL);
  }
}
