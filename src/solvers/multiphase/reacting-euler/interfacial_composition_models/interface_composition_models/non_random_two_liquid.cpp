// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "non_random_two_liquid.hpp"
// Constructors 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::NonRandomTwoLiquid<Thermo, OtherThermo>::
NonRandomTwoLiquid
(
  const dictionary& dict,
  const phasePair& pair
)
:
  InterfaceCompositionModel<Thermo, OtherThermo>{dict, pair},
  gamma1_
  {
    {
      IOobject::groupName("gamma1", pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh()
    },
    pair.phase1().mesh(),
    {"one", dimless, 1}
  },
  gamma2_
  {
    {
      IOobject::groupName("gamma2", pair.name()),
      pair.phase1().mesh().time().timeName(),
      pair.phase1().mesh()
    },
    pair.phase1().mesh(),
    {"one", dimless, 1}
  },
  beta12_{"beta12", dimless/dimTemperature, 0},
  beta21_{"beta21", dimless/dimTemperature, 0}
{
  if (this->speciesNames_.size() != 2)
  {
    FATAL_ERROR_IN
    (
      "template<class Thermo, class OtherThermo>"
      "mousse::interfaceCompositionModels::"
      "NonRandomTwoLiquid<Thermo, OtherThermo>::"
      "NonRandomTwoLiquid"
      "( "
        "const dictionary& dict, "
        "const phasePair& pair "
      ")"
    )   << "NonRandomTwoLiquid model is suitable for two species only."
      << exit(FatalError);
  }
  species1Name_ = this->speciesNames_[0];
  species2Name_ = this->speciesNames_[1];
  species1Index_ = this->thermo_.composition().species()[species1Name_];
  species2Index_ = this->thermo_.composition().species()[species2Name_];
  alpha12_ = dimensionedScalar
  {
    "alpha12",
    dimless,
    dict.subDict(species1Name_).lookup("alpha")
  };
  alpha21_ = dimensionedScalar
  {
    "alpha21",
    dimless,
    dict.subDict(species2Name_).lookup("alpha")
  };
  beta12_ = dimensionedScalar
  {
    "beta12",
    dimless/dimTemperature,
    dict.subDict(species1Name_).lookup("beta")
  };
  beta21_ = dimensionedScalar
  {
    "beta21",
    dimless/dimTemperature,
    dict.subDict(species2Name_).lookup("beta")
  };
  saturationModel12_.reset
  (
    saturationModel::New
    (
      dict.subDict(species1Name_).subDict("interaction")
    ).ptr()
  );
  saturationModel21_.reset
  (
    saturationModel::New
    (
      dict.subDict(species2Name_).subDict("interaction")
    ).ptr()
  );
  speciesModel1_.reset
  (
    interfaceCompositionModel::New
    (
      dict.subDict(species1Name_),
      pair
    ).ptr()
  );
  speciesModel2_.reset
  (
    interfaceCompositionModel::New
    (
      dict.subDict(species2Name_),
      pair
    ).ptr()
  );
}
// Destructor 
template<class Thermo, class OtherThermo>
mousse::interfaceCompositionModels::NonRandomTwoLiquid<Thermo, OtherThermo>::
~NonRandomTwoLiquid()
{}
// Member Functions 
template<class Thermo, class OtherThermo>
void
mousse::interfaceCompositionModels::NonRandomTwoLiquid<Thermo, OtherThermo>::
update
(
  const volScalarField& Tf
)
{
  volScalarField W{this->thermo_.composition().W()};
  volScalarField X1
  {
    this->thermo_.composition().Y(species1Index_)*W
    /this->thermo_.composition().W(species1Index_)
  };
  volScalarField X2
  {
    this->thermo_.composition().Y(species2Index_)*W
    /this->thermo_.composition().W(species2Index_)
  };
  volScalarField alpha12{alpha12_ + Tf*beta12_};
  volScalarField alpha21{alpha21_ + Tf*beta21_};
  volScalarField tau12{saturationModel12_->lnPSat(Tf)};
  volScalarField tau21{saturationModel21_->lnPSat(Tf)};
  volScalarField G12{exp(- alpha12*tau12)};
  volScalarField G21{exp(- alpha21*tau21)};
  gamma1_ =
    exp
    (
      sqr(X2)
      *(tau21*sqr(G21)/max(sqr(X1 + X2*G21), SMALL)
        + tau12*G12/max(sqr(X2 + X1*G12), SMALL))
    );
  gamma2_ =
    exp
    (
      sqr(X1)
      *(tau12*sqr(G12)/max(sqr(X2 + X1*G12), SMALL)
        + tau21*G21/max(sqr(X1 + X2*G21), SMALL))
    );
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::NonRandomTwoLiquid<Thermo, OtherThermo>::Yf
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (speciesName == species1Name_)
  {
    return
      this->otherThermo_.composition().Y(speciesName)
      *speciesModel1_->Yf(speciesName, Tf)
      *gamma1_;
  }
  else if(speciesName == species2Name_)
  {
    return
      this->otherThermo_.composition().Y(speciesName)
      *speciesModel2_->Yf(speciesName, Tf)
      *gamma2_;
  }
  else
  {
    return
      this->thermo_.composition().Y(speciesName)
      *(scalar(1) - Yf(species1Name_, Tf) - Yf(species2Name_, Tf));
  }
}
template<class Thermo, class OtherThermo>
mousse::tmp<mousse::volScalarField>
mousse::interfaceCompositionModels::NonRandomTwoLiquid<Thermo, OtherThermo>::
YfPrime
(
  const word& speciesName,
  const volScalarField& Tf
) const
{
  if (speciesName == species1Name_)
  {
    return
      this->otherThermo_.composition().Y(speciesName)
      *speciesModel1_->YfPrime(speciesName, Tf)
      *gamma1_;
  }
  else if(speciesName == species2Name_)
  {
    return
      this->otherThermo_.composition().Y(speciesName)
      *speciesModel2_->YfPrime(speciesName, Tf)
      *gamma2_;
  }
  else
  {
    return -this->thermo_.composition().Y(speciesName)
      *(YfPrime(species1Name_, Tf) + YfPrime(species2Name_, Tf));
  }
}
