// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "raoult.hpp"
// Constructors 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Raoult<Thermo, OtherThermo>::Raoult
(
  const dictionary& dict,
  const phasePair& pair
)
:
  InterfaceCompositionModel<Thermo, OtherThermo>{dict, pair},
  YNonVapour_
  {
    {
      IOobject::groupName("YNonVapour", pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh()
    },
    pair.phase1().mesh(),
    {"one", dimless, 1}
  },
  YNonVapourPrime_
  {
    {
      IOobject::groupName("YNonVapourPrime", pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh()
    },
    pair.phase1().mesh(),
    {"zero", dimless/dimTemperature, 0}
  }
{
  FOR_ALL_CONST_ITER(hashedWordList, this->speciesNames_, iter)
  {
    speciesModels_.insert
    (
      *iter,
      autoPtr<interfaceCompositionModel>
      {
        interfaceCompositionModel::New
        (
          dict.subDict(*iter),
          pair
        )
      }
    );
  }
}
// Destructor 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::Raoult<Thermo, OtherThermo>::~Raoult()
{}
// Member Functions 
template<class Thermo, class OtherThermo>
void mousse::interfaceCompositionModels::Raoult<Thermo, OtherThermo>::update
(
  const volScalarField& Tf
)
{
  YNonVapour_ = scalar(1);
  FOR_ALL_ITER
  (
    HashTable<autoPtr<interfaceCompositionModel>>,
    speciesModels_,
    iter
  )
  {
    iter()->update(Tf);
    YNonVapour_ -= this->otherThermo_.composition().Y(iter.key())
      *iter()->Yf(iter.key(), Tf);
    YNonVapourPrime_ -= this->otherThermo_.composition().Y(iter.key())
      *iter()->YfPrime(iter.key(), Tf);
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Raoult<Thermo, OtherThermo>::Yf
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (this->speciesNames_.contains(speciesName))
  {
    return this->otherThermo_.composition().Y(speciesName)
      *speciesModels_[speciesName]->Yf(speciesName, Tf);
  }
  else
  {
    return this->thermo_.composition().Y(speciesName)
      *YNonVapour_;
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::Raoult<Thermo, OtherThermo>::YfPrime
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (this->speciesNames_.contains(speciesName))
  {
    return this->otherThermo_.composition().Y(speciesName)
      *speciesModels_[speciesName]->YfPrime(speciesName, Tf);
  }
  else
  {
    return this->otherThermo_.composition().Y(speciesName)
      *YNonVapourPrime_;
  }
}
