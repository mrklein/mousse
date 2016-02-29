// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kts.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiGModels
{
DEFINE_TYPE_NAME_AND_DEBUG(KTS, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiGModel, KTS, dictionary);
}
}
// Constructors 
mousse::XiGModels::KTS::KTS
(
  const dictionary& XiGProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiGModel{XiGProperties, thermo, turbulence, Su},
  GEtaCoef_{readScalar(XiGModelCoeffs_.lookup("GEtaCoef"))}
{}
// Destructor 
mousse::XiGModels::KTS::~KTS()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiGModels::KTS::G() const
{
  volScalarField up{sqrt((2.0/3.0)*turbulence_.k())};
  const volScalarField& epsilon = turbulence_.epsilon();
  volScalarField tauEta{sqrt(mag(thermo_.muu()/(thermo_.rhou()*epsilon)))};
  return (GEtaCoef_/tauEta);
}
bool mousse::XiGModels::KTS::read(const dictionary& XiGProperties)
{
  XiGModel::read(XiGProperties);
  XiGModelCoeffs_.lookup("GEtaCoef") >> GEtaCoef_;
  return true;
}
