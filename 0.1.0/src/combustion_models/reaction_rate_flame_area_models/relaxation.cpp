// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "relaxation.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fvm.hpp"
#include "les_model.hpp"
// Static Data Members
namespace mousse
{
namespace reactionRateFlameAreaModels
{
  DEFINE_TYPE_NAME_AND_DEBUG(relaxation, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    reactionRateFlameArea,
    relaxation,
    dictionary
  );
}
}
// Constructors 
mousse::reactionRateFlameAreaModels::relaxation::relaxation
(
  const word modelType,
  const dictionary& dict,
  const fvMesh& mesh,
  const combustionModel& combModel
)
:
  reactionRateFlameArea(modelType, dict, mesh, combModel),
  correlation_(dict.subDict(typeName + "Coeffs").subDict(fuel_)),
  C_(readScalar(dict.subDict(typeName + "Coeffs").lookup("C"))),
  alpha_(readScalar(dict.subDict(typeName + "Coeffs").lookup("alpha")))
{}
// Destructor 
mousse::reactionRateFlameAreaModels::relaxation::~relaxation()
{}
// Member Functions 
void mousse::reactionRateFlameAreaModels::relaxation::correct
(
  const volScalarField& sigma
)
{
  dimensionedScalar omega0
  (
    "omega0",
    dimensionSet(1, -2, -1, 0, 0, 0, 0),
    correlation_.omega0()
  );
  dimensionedScalar sigmaExt
  (
    "sigmaExt",
    dimensionSet(0, 0, -1, 0, 0, 0, 0),
    correlation_.sigmaExt()
  );
  dimensionedScalar omegaMin
  (
    "omegaMin",
    omega0.dimensions(),
    1e-4
  );
  const compressible::LESModel& lesModel =
    omega_.db().lookupObject<compressible::LESModel>("LESProperties");
  // Total strain : resolved and sub-grid (just LES for now)
  const volScalarField sigmaTotal
  (
    sigma + alpha_*lesModel.epsilon()/(lesModel.k() + lesModel.kMin())
  );
  const volScalarField omegaInf(correlation_.omega0Sigma(sigmaTotal));
  dimensionedScalar sigma0("sigma0", sigma.dimensions(), 0.0);
  const volScalarField tau(C_*mag(sigmaTotal));
  volScalarField Rc
  (
    (tau*omegaInf*(omega0 - omegaInf) + sqr(omegaMin)*sigmaExt)
   /(sqr(omega0 - omegaInf) + sqr(omegaMin))
  );
  const volScalarField rho(combModel_.rho());
  const surfaceScalarField phi(combModel_.phi());
  solve
  (
    fvm::ddt(rho, omega_)
   + fvm::div(phi, omega_, "div(phi,omega)")
   ==
    rho*Rc*omega0
   - fvm::SuSp(rho*(tau + Rc), omega_)
  );
  omega_.min(omega0);
  omega_.max(0.0);
}
bool  mousse::reactionRateFlameAreaModels::relaxation::read
(
  const dictionary& dict
)
{
  if (reactionRateFlameArea::read(dict))
  {
    coeffDict_ = dict.subDict(typeName + "Coeffs");
    coeffDict_.lookup("C") >> C_;
    coeffDict_.lookup("alpha") >> alpha_;
    correlation_.read
    (
      coeffDict_.subDict(fuel_)
    );
    return true;
  }
  else
  {
    return false;
  }
}
