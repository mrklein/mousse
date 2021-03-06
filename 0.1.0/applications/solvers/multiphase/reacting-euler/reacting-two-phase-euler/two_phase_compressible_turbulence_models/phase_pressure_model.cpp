// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_pressure_model.hpp"
#include "two_phase_system.hpp"
// Constructors 
mousse::RASModels::phasePressureModel::phasePressureModel
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const volVectorField& U,
  const surfaceScalarField& alphaRhoPhi,
  const surfaceScalarField& phi,
  const transportModel& phase,
  const word& propertiesName,
  const word& type
)
:
  eddyViscosity
  <
    RASModel<EddyDiffusivity<ThermalDiffusivity
    <
      PhaseCompressibleTurbulenceModel<phaseModel>
    >>>
  >
  (
    type,
    alpha,
    rho,
    U,
    alphaRhoPhi,
    phi,
    phase,
    propertiesName
  ),
  // phase_{phase},
  alphaMax_{readScalar(coeffDict_.lookup("alphaMax"))},
  preAlphaExp_{readScalar(coeffDict_.lookup("preAlphaExp"))},
  expMax_{readScalar(coeffDict_.lookup("expMax"))},
  g0_
  {
    "g0",
    {1, -1, -2, 0, 0},
    coeffDict_.lookup("g0")
  }
{
  nut_ == dimensionedScalar{"zero", nut_.dimensions(), 0.0};
  if (type == typeName)
  {
    printCoeffs(type);
  }
}
// Destructor 
mousse::RASModels::phasePressureModel::~phasePressureModel()
{}
// Member Functions 
bool mousse::RASModels::phasePressureModel::read()
{
  if
  (
    eddyViscosity
    <
      RASModel<EddyDiffusivity<ThermalDiffusivity
      <
        PhaseCompressibleTurbulenceModel<phaseModel>
      >>>
    >::read()
  )
  {
    coeffDict().lookup("alphaMax") >> alphaMax_;
    coeffDict().lookup("preAlphaExp") >> preAlphaExp_;
    coeffDict().lookup("expMax") >> expMax_;
    g0_.readIfPresent(coeffDict());
    return true;
  }
  else
  {
    return false;
  }
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::phasePressureModel::k() const
{
  NOT_IMPLEMENTED("phasePressureModel::k()");
  return nut_;
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::phasePressureModel::epsilon() const
{
  NOT_IMPLEMENTED("phasePressureModel::epsilon()");
  return nut_;
}
mousse::tmp<mousse::volSymmTensorField>
mousse::RASModels::phasePressureModel::R() const
{
  return tmp<volSymmTensorField>
  {
    new volSymmTensorField
    {
      {
        IOobject::groupName("R", U_.group()),
        runTime_.timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      dimensioned<symmTensor>
      {
        "R",
        {0, 2, -2, 0, 0},
        symmTensor::zero
      }
    }
  };
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::phasePressureModel::pPrime() const
{
  tmp<volScalarField> tpPrime
  {
    g0_*min(exp(preAlphaExp_*(alpha_ - alphaMax_)),
            expMax_)
  };
  volScalarField::GeometricBoundaryField& bpPrime = tpPrime().boundaryField();
  FOR_ALL(bpPrime, patchi)
  {
    if (!bpPrime[patchi].coupled())
    {
      bpPrime[patchi] == 0;
    }
  }
  return tpPrime;
}
mousse::tmp<mousse::surfaceScalarField>
mousse::RASModels::phasePressureModel::pPrimef() const
{
  tmp<surfaceScalarField> tpPrime
  {
    g0_*min(exp(preAlphaExp_*(fvc::interpolate(alpha_) - alphaMax_)),
            expMax_)
  };
 surfaceScalarField::GeometricBoundaryField& bpPrime =
   tpPrime().boundaryField();
  FOR_ALL(bpPrime, patchi)
  {
    if (!bpPrime[patchi].coupled())
    {
      bpPrime[patchi] == 0;
    }
  }
  return tpPrime;
}
mousse::tmp<mousse::volSymmTensorField>
mousse::RASModels::phasePressureModel::devRhoReff() const
{
  return tmp<volSymmTensorField>
  {
    new volSymmTensorField
    {
      {
        IOobject::groupName("devRhoReff", U_.group()),
        runTime_.timeName(),
        mesh_,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      mesh_,
      dimensioned<symmTensor>
      {
        "R",
        rho_.dimensions()*dimensionSet(0, 2, -2, 0, 0),
        symmTensor::zero
      }
    }
  };
}
mousse::tmp<mousse::fvVectorMatrix>
mousse::RASModels::phasePressureModel::divDevRhoReff
(
  volVectorField& U
) const
{
  return tmp<fvVectorMatrix>
  {
    new fvVectorMatrix
    {
      U,
      rho_.dimensions()*dimensionSet(0, 4, -2, 0, 0)
    }
  };
}
void mousse::RASModels::phasePressureModel::correct()
{}
