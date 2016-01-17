// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "no_pyrolysis.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "absorption_emission_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace pyrolysisModels
{
// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(noPyrolysis, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(pyrolysisModel, noPyrolysis, mesh);
ADD_TO_RUN_TIME_SELECTION_TABLE(pyrolysisModel, noPyrolysis, dictionary);
// Protected Member Functions 
void noPyrolysis::constructThermoChemistry()
{
  solidChemistry_.reset
  (
    basicSolidChemistryModel::New(regionMesh()).ptr()
  );
  solidThermo_.reset(&solidChemistry_->solidThermo());
  radiation_.reset(radiation::radiationModel::New(solidThermo_->T()).ptr());
}
bool noPyrolysis::read()
{
  if (pyrolysisModel::read())
  {
    // no additional info to read
    return true;
  }
  else
  {
    return false;
  }
}
bool noPyrolysis::read(const dictionary& dict)
{
  if (pyrolysisModel::read(dict))
  {
    // no additional info to read
    return true;
  }
  else
  {
    return false;
  }
}
// Constructors 
noPyrolysis::noPyrolysis
(
  const word& /*modelType*/,
  const fvMesh& mesh,
  const word& regionType
)
:
  pyrolysisModel{mesh, regionType},
  solidChemistry_{NULL},
  solidThermo_{NULL},
  radiation_{NULL}
{
  if (active())
  {
    constructThermoChemistry();
  }
}
noPyrolysis::noPyrolysis
(
  const word& /*modelType*/,
  const fvMesh& mesh,
  const dictionary& /*dict*/,
  const word& regionType
)
:
  pyrolysisModel{mesh, regionType},
  solidChemistry_{NULL},
  solidThermo_{NULL},
  radiation_{NULL}
{
  if (active())
  {
    constructThermoChemistry();
  }
}
// Destructor 
noPyrolysis::~noPyrolysis()
{}
// Member Functions 
void noPyrolysis::preEvolveRegion()
{
  //Do nothing
}
void noPyrolysis::evolveRegion()
{
  //Do nothing
}
const volScalarField& noPyrolysis::rho() const
{
  return solidThermo_->rho();
}
const volScalarField& noPyrolysis::T() const
{
  return solidThermo_->T();
}
const tmp<volScalarField> noPyrolysis::Cp() const
{
  return solidThermo_->Cp();
}
tmp<volScalarField> noPyrolysis::kappaRad() const
{
  return radiation_->absorptionEmission().a();
}
tmp<volScalarField> noPyrolysis::kappa() const
{
  return solidThermo_->kappa();
}
const surfaceScalarField& noPyrolysis::phiGas() const
{
  FATAL_ERROR_IN("const volScalarField& noPyrolysis::phiGas() const")
    << "phiGas field not available for " << type() << abort(FatalError);
  return surfaceScalarField::null();
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
