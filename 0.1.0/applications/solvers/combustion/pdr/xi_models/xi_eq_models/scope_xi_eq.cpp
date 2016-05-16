// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scope_xi_eq.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {
namespace XiEqModels {

DEFINE_TYPE_NAME_AND_DEBUG(SCOPEXiEq, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiEqModel, SCOPEXiEq, dictionary);

}
}


// Constructors 
mousse::XiEqModels::SCOPEXiEq::SCOPEXiEq
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModel{XiEqProperties, thermo, turbulence, Su},
  XiEqCoef_{readScalar(XiEqModelCoeffs_.lookup("XiEqCoef"))},
  XiEqExp_{readScalar(XiEqModelCoeffs_.lookup("XiEqExp"))},
  lCoef_{readScalar(XiEqModelCoeffs_.lookup("lCoef"))},
  SuMin_{0.01*Su.average()},
  uPrimeCoef_{readScalar(XiEqModelCoeffs_.lookup("uPrimeCoef"))},
  subGridSchelkin_{readBool(XiEqModelCoeffs_.lookup("subGridSchelkin"))},
  MaModel
  {
    Su.mesh().lookupObject<IOdictionary>("combustionProperties"),
    thermo
  }
{}


// Destructor 
mousse::XiEqModels::SCOPEXiEq::~SCOPEXiEq()
{}


// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiEqModels::SCOPEXiEq::XiEq() const
{
  const volScalarField& k = turbulence_.k();
  const volScalarField& epsilon = turbulence_.epsilon();
  volScalarField up{sqrt((2.0/3.0)*k)};
  if (subGridSchelkin_) {
    up.internalField() += calculateSchelkinEffect(uPrimeCoef_);
  }
  volScalarField l{lCoef_*sqrt(3.0/2.0)*up*k/epsilon};
  volScalarField Rl{up*l*thermo_.rhou()/thermo_.muu()};
  volScalarField upBySu{up/(Su_ + SuMin_)};
  volScalarField K{0.157*upBySu/sqrt(Rl)};
  volScalarField Ma{MaModel.Ma()};
  tmp<volScalarField> tXiEq
  {
    new volScalarField
    {
      {
        "XiEq",
        epsilon.time().timeName(),
        epsilon.db()
      },
      epsilon.mesh(),
      {"XiEq", dimless, 0.0}
    }
  };
  volScalarField& xieq = tXiEq();
  FOR_ALL(xieq, celli) {
    if (Ma[celli] > 0.01) {
      xieq[celli] = XiEqCoef_*pow(K[celli]*Ma[celli], -XiEqExp_)*upBySu[celli];
    }
  }
  FOR_ALL(xieq.boundaryField(), patchi) {
    scalarField& xieqp = xieq.boundaryField()[patchi];
    const scalarField& Kp = K.boundaryField()[patchi];
    const scalarField& Map = Ma.boundaryField()[patchi];
    const scalarField& upBySup = upBySu.boundaryField()[patchi];
    FOR_ALL(xieqp, facei) {
      if (Ma[facei] > 0.01) {
        xieqp[facei] =
          XiEqCoef_*pow(Kp[facei]*Map[facei], -XiEqExp_)*upBySup[facei];
      }
    }
  }
  return tXiEq;
}


bool mousse::XiEqModels::SCOPEXiEq::read(const dictionary& XiEqProperties)
{
  XiEqModel::read(XiEqProperties);
  XiEqModelCoeffs_.lookup("XiEqCoef") >> XiEqCoef_;
  XiEqModelCoeffs_.lookup("XiEqExp") >> XiEqExp_;
  XiEqModelCoeffs_.lookup("lCoef") >> lCoef_;
  XiEqModelCoeffs_.lookup("uPrimeCoef") >> uPrimeCoef_;
  XiEqModelCoeffs_.lookup("subGridSchelkin") >> subGridSchelkin_;
  return true;
}

