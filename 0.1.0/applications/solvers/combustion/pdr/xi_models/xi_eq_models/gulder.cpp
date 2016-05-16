// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gulder.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace XiEqModels {

DEFINE_TYPE_NAME_AND_DEBUG(Gulder, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiEqModel, Gulder, dictionary);

}
}


// Constructors 
mousse::XiEqModels::Gulder::Gulder
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModel{XiEqProperties, thermo, turbulence, Su},
  XiEqCoef_{readScalar(XiEqModelCoeffs_.lookup("XiEqCoef"))},
  SuMin_{0.01*Su.average()},
  uPrimeCoef_{readScalar(XiEqModelCoeffs_.lookup("uPrimeCoef"))},
  subGridSchelkin_{readBool(XiEqModelCoeffs_.lookup("subGridSchelkin"))}
{}


// Destructor 
mousse::XiEqModels::Gulder::~Gulder()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiEqModels::Gulder::XiEq() const
{
  volScalarField up{sqrt((2.0/3.0)*turbulence_.k())};
  const volScalarField& epsilon = turbulence_.epsilon();
  if (subGridSchelkin_) {
    up.internalField() += calculateSchelkinEffect(uPrimeCoef_);
  }
  volScalarField tauEta{sqrt(mag(thermo_.muu()/(thermo_.rhou()*epsilon)))};
  volScalarField Reta
  {
    up/(sqrt(epsilon*tauEta) + dimensionedScalar{"1e-8", up.dimensions(), 1e-8})
  };
  return (1.0 + XiEqCoef_*sqrt(up/(Su_ + SuMin_))*Reta);
}


bool mousse::XiEqModels::Gulder::read(const dictionary& XiEqProperties)
{
  XiEqModel::read(XiEqProperties);
  XiEqModelCoeffs_.lookup("XiEqCoef") >> XiEqCoef_;
  XiEqModelCoeffs_.lookup("uPrimeCoef") >> uPrimeCoef_;
  XiEqModelCoeffs_.lookup("subGridSchelkin") >> subGridSchelkin_;
  return true;
}

