// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant_film_thermo.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Static Data Members
DEFINE_TYPE_NAME_AND_DEBUG(constantFilmThermo, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  filmThermoModel,
  constantFilmThermo,
  dictionary
);


void constantFilmThermo::init(thermoData& td)
{
  if (coeffDict_.readIfPresent(td.name, td.value)) {
    td.set = true;
  }
}


// Constructors 
constantFilmThermo::constantFilmThermo
(
  surfaceFilmModel& owner,
  const dictionary& dict
)
:
  filmThermoModel(typeName, owner, dict),
  name_(coeffDict_.lookup("specieName")),
  rho0_("rho0"),
  mu0_("mu0"),
  sigma0_("sigma0"),
  Cp0_("Cp0"),
  kappa0_("kappa0"),
  hl0_("hl0"),
  pv0_("pv0"),
  W0_("W0"),
  Tb0_("Tb0")
{
  init(rho0_);
  init(mu0_);
  init(sigma0_);
  init(Cp0_);
  init(kappa0_);
  init(hl0_);
  init(pv0_);
  init(W0_);
  init(Tb0_);
}


// Destructor 
constantFilmThermo::~constantFilmThermo()
{}


// Member Functions 
const word& constantFilmThermo::name() const
{
  return name_;
}


scalar constantFilmThermo::rho
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!rho0_.set) {
    coeffDict_.lookup(rho0_.name) >> rho0_.value;
    rho0_.set = true;
  }
  return rho0_.value;
}


scalar constantFilmThermo::mu
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!mu0_.set) {
    coeffDict_.lookup(mu0_.name) >> mu0_.value;
    mu0_.set = true;
  }
  return mu0_.value;
}


scalar constantFilmThermo::sigma
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!sigma0_.set) {
    coeffDict_.lookup(sigma0_.name) >> sigma0_.value;
    sigma0_.set = true;
  }
  return sigma0_.value;
}


scalar constantFilmThermo::Cp
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!Cp0_.set) {
    coeffDict_.lookup(Cp0_.name) >> Cp0_.value;
    Cp0_.set = true;
  }
  return Cp0_.value;
}


scalar constantFilmThermo::kappa
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!kappa0_.set) {
    coeffDict_.lookup(kappa0_.name) >> kappa0_.value;
    kappa0_.set = true;
  }
  return kappa0_.value;
}


scalar constantFilmThermo::D
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!D0_.set) {
    coeffDict_.lookup(D0_.name) >> D0_.value;
    D0_.set = true;
  }
  return D0_.value;
}


scalar constantFilmThermo::hl
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!hl0_.set) {
    coeffDict_.lookup(hl0_.name) >> hl0_.value;
    hl0_.set = true;
  }
  return hl0_.value;
}


scalar constantFilmThermo::pv
(
  const scalar /*p*/,
  const scalar /*T*/
) const
{
  if (!pv0_.set) {
    coeffDict_.lookup(pv0_.name) >> pv0_.value;
    pv0_.set = true;
  }
  return pv0_.value;
}


scalar constantFilmThermo::W() const
{
  if (!W0_.set) {
    coeffDict_.lookup(W0_.name) >> W0_.value;
    W0_.set = true;
  }
  return W0_.value;
}


scalar constantFilmThermo::Tb(const scalar /*p*/) const
{
  if (!Tb0_.set) {
    coeffDict_.lookup(Tb0_.name) >> Tb0_.value;
    Tb0_.set = true;
  }
  return Tb0_.value;
}


tmp<volScalarField> constantFilmThermo::rho() const
{
  tmp<volScalarField> trho
  {
    new volScalarField
    {
      // IOobject
      {
        type() + ':' + rho0_.name,
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"0", dimDensity, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  trho().internalField() = this->rho(0, 0);
  trho().correctBoundaryConditions();
  return trho;
}


tmp<volScalarField> constantFilmThermo::mu() const
{
  tmp<volScalarField> tmu
  {
    new volScalarField
    {
      {
        type() + ':' + mu0_.name,
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"0", dimPressure*dimTime, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  tmu().internalField() = this->mu(0, 0);
  tmu().correctBoundaryConditions();
  return tmu;
}


tmp<volScalarField> constantFilmThermo::sigma() const
{
  tmp<volScalarField> tsigma
  {
    new volScalarField
    {
      {
        type() + ':' + sigma0_.name,
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"0", dimMass/sqr(dimTime), 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  tsigma().internalField() = this->sigma(0, 0);
  tsigma().correctBoundaryConditions();
  return tsigma;
}


tmp<volScalarField> constantFilmThermo::Cp() const
{
  tmp<volScalarField> tCp
  {
    new volScalarField
    {
      {
        type() + ':' + Cp0_.name,
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"0", dimEnergy/dimMass/dimTemperature, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  tCp().internalField() = this->Cp(0, 0);
  tCp().correctBoundaryConditions();
  return tCp;
}


tmp<volScalarField> constantFilmThermo::kappa() const
{
  tmp<volScalarField> tkappa
  {
    new volScalarField
    {
      {
        type() + ':' + kappa0_.name,
        owner().time().timeName(),
        owner().regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      owner().regionMesh(),
      {"0", dimPower/dimLength/dimTemperature, 0.0},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  tkappa().internalField() = this->kappa(0, 0);
  tkappa().correctBoundaryConditions();
  return tkappa;
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

