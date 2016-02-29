// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "kinetic_theory_model.hpp"
#include "mathematical_constants.hpp"
#include "two_phase_system.hpp"
// Constructors 
mousse::RASModels::kineticTheoryModel::kineticTheoryModel
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
    RASModel<EddyDiffusivity<phaseCompressibleTurbulenceModel>>
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
  phase_{phase},
  viscosityModel_
  {
    kineticTheoryModels::viscosityModel::New
    (
      coeffDict_
    )
  },
  conductivityModel_
  {
    kineticTheoryModels::conductivityModel::New
    (
      coeffDict_
    )
  },
  radialModel_
  {
    kineticTheoryModels::radialModel::New
    (
      coeffDict_
    )
  },
  granularPressureModel_
  {
    kineticTheoryModels::granularPressureModel::New
    (
      coeffDict_
    )
  },
  frictionalStressModel_
  {
    kineticTheoryModels::frictionalStressModel::New
    (
      coeffDict_
    )
  },
  equilibrium_{coeffDict_.lookup("equilibrium")},
  e_{"e", dimless, coeffDict_},
  alphaMax_{"alphaMax", dimless, coeffDict_},
  alphaMinFriction_
  {
    "alphaMinFriction",
    dimless,
    coeffDict_
  },
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    coeffDict_
  },
  Theta_
  {
    {
      IOobject::groupName("Theta", phase.name()),
      U.time().timeName(),
      U.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    U.mesh()
  },
  lambda_
  {
    {
      IOobject::groupName("lambda", phase.name()),
      U.time().timeName(),
      U.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    U.mesh(),
    {"zero", dimensionSet(0, 2, -1, 0, 0), 0.0}
  },
  gs0_
  {
    {
      IOobject::groupName("gs0", phase.name()),
      U.time().timeName(),
      U.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    U.mesh(),
    {"zero", dimensionSet(0, 0, 0, 0, 0), 0.0}
  },
  kappa_
  {
    {
      IOobject::groupName("kappa", phase.name()),
      U.time().timeName(),
      U.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    U.mesh(),
    {"zero", dimensionSet(1, -1, -1, 0, 0), 0.0}
  }
{
  if (type == typeName)
  {
    printCoeffs(type);
  }
}
// Destructor 
mousse::RASModels::kineticTheoryModel::~kineticTheoryModel()
{}
// Member Functions 
bool mousse::RASModels::kineticTheoryModel::read()
{
  if
  (
    eddyViscosity
    <
      RASModel<EddyDiffusivity<phaseCompressibleTurbulenceModel> >
    >::read()
  )
  {
    coeffDict().lookup("equilibrium") >> equilibrium_;
    e_.readIfPresent(coeffDict());
    alphaMax_.readIfPresent(coeffDict());
    alphaMinFriction_.readIfPresent(coeffDict());
    viscosityModel_->read();
    conductivityModel_->read();
    radialModel_->read();
    granularPressureModel_->read();
    frictionalStressModel_->read();
    return true;
  }
  else
  {
    return false;
  }
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::kineticTheoryModel::k() const
{
  NOT_IMPLEMENTED("kineticTheoryModel::k()");
  return nut_;
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::kineticTheoryModel::epsilon() const
{
  NOT_IMPLEMENTED("kineticTheoryModel::epsilon()");
  return nut_;
}
mousse::tmp<mousse::volSymmTensorField>
mousse::RASModels::kineticTheoryModel::R() const
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
        -(nut_)*dev(twoSymm(fvc::grad(U_)))
        - (lambda_*fvc::div(phi_))*symmTensor::I
    }
  };
}
mousse::tmp<mousse::volScalarField>
mousse::RASModels::kineticTheoryModel::pPrime() const
{
  const volScalarField& rho = phase_.rho();
  tmp<volScalarField> tpPrime
  {
    Theta_
    *granularPressureModel_->granularPressureCoeffPrime
    (
      alpha_,
      radialModel_->g0(alpha_, alphaMinFriction_, alphaMax_),
      radialModel_->g0prime(alpha_, alphaMinFriction_, alphaMax_),
      rho,
      e_
    )
    + frictionalStressModel_->frictionalPressurePrime
    (
      alpha_,
      alphaMinFriction_,
      alphaMax_
    )
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
mousse::RASModels::kineticTheoryModel::pPrimef() const
{
  return fvc::interpolate(pPrime());
}
mousse::tmp<mousse::volSymmTensorField>
mousse::RASModels::kineticTheoryModel::devRhoReff() const
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
      -(rho_*nut_)*dev(twoSymm(fvc::grad(U_)))
      - ((rho_*lambda_)*fvc::div(phi_))*symmTensor::I
    }
  };
}
mousse::tmp<mousse::fvVectorMatrix>
mousse::RASModels::kineticTheoryModel::divDevRhoReff
(
  volVectorField& U
) const
{
  return
  (
    - fvm::laplacian(rho_*nut_, U)
    - fvc::div
    ((rho_*nut_)*dev2(T(fvc::grad(U)))
     + ((rho_*lambda_)*fvc::div(phi_))
     *dimensioned<symmTensor>("I", dimless, symmTensor::I))
  );
}
void mousse::RASModels::kineticTheoryModel::correct()
{
  // Local references
  volScalarField alpha{max(alpha_, scalar(0))};
  const volScalarField& rho = phase_.rho();
  const surfaceScalarField& alphaRhoPhi = alphaRhoPhi_;
  const volVectorField& U = U_;
  const volVectorField& Uc_ =
    refCast<const twoPhaseSystem>(phase_.fluid()).otherPhase(phase_).U();
  const scalar sqrtPi = sqrt(constant::mathematical::pi);
  dimensionedScalar ThetaSmall{"ThetaSmall", Theta_.dimensions(), 1.0e-6};
  dimensionedScalar ThetaSmallSqrt{sqrt(ThetaSmall)};
  tmp<volScalarField> tda{phase_.d()};
  const volScalarField& da = tda();
  tmp<volTensorField> tgradU{fvc::grad(U_)};
  const volTensorField& gradU = tgradU();
  volSymmTensorField D{symm(gradU)};
  // Calculating the radial distribution function
  gs0_ = radialModel_->g0(alpha, alphaMinFriction_, alphaMax_);
  if (!equilibrium_)
  {
    // Particle viscosity (Table 3.2, p.47)
    nut_ = viscosityModel_->nu(alpha, Theta_, gs0_, rho, da, e_);
    volScalarField ThetaSqrt{"sqrtTheta", sqrt(Theta_)};
    // Bulk viscosity  p. 45 (Lun et al. 1984).
    lambda_ = (4.0/3.0)*sqr(alpha)*da*gs0_*(1.0 + e_)*ThetaSqrt/sqrtPi;
    // Stress tensor, Definitions, Table 3.1, p. 43
    volSymmTensorField tau
    {
      rho*(2.0*nut_*D + (lambda_ - (2.0/3.0)*nut_)*tr(D)*I)
    };
    // Dissipation (Eq. 3.24, p.50)
    volScalarField gammaCoeff
    {
      "gammaCoeff",
      12.0*(1.0 - sqr(e_))*max(sqr(alpha), residualAlpha_)
      *rho*gs0_*(1.0/da)*ThetaSqrt/sqrtPi
    };
    // Drag
    volScalarField beta
    {
      refCast<const twoPhaseSystem>(phase_.fluid()).drag(phase_).K()
    };
    // Eq. 3.25, p. 50 Js = J1 - J2
    volScalarField J1{"J1", 3.0*beta};
    volScalarField J2
    {
      "J2",
      0.25*sqr(beta)*da*magSqr(U - Uc_)
      /(max(alpha, residualAlpha_)*rho*sqrtPi*(ThetaSqrt + ThetaSmallSqrt))
    };
    // particle pressure - coefficient in front of Theta (Eq. 3.22, p. 45)
    volScalarField PsCoeff
    {
      granularPressureModel_->granularPressureCoeff
      (
        alpha,
        gs0_,
        rho,
        e_
      )
    };
    // 'thermal' conductivity (Table 3.3, p. 49)
    kappa_ = conductivityModel_->kappa(alpha, Theta_, gs0_, rho, da, e_);
    // Construct the granular temperature equation (Eq. 3.20, p. 44)
    // NB. note that there are two typos in Eq. 3.20:
    //     Ps should be without grad
    //     the laplacian has the wrong sign
    fvScalarMatrix ThetaEqn
    {
      1.5*
      (
        fvm::ddt(alpha, rho, Theta_)
        + fvm::div(alphaRhoPhi, Theta_)
        - fvc::Sp(fvc::ddt(alpha, rho) + fvc::div(alphaRhoPhi), Theta_)
      )
      - fvm::laplacian(kappa_, Theta_, "laplacian(kappa,Theta)")
      ==
      fvm::SuSp(-((PsCoeff*I) && gradU), Theta_)
      + (tau && gradU)
      + fvm::Sp(-gammaCoeff, Theta_)
      + fvm::Sp(-J1, Theta_)
      + fvm::Sp(J2/(Theta_ + ThetaSmall), Theta_)
    };
    ThetaEqn.relax();
    ThetaEqn.solve();
  }
  else
  {
    // Equilibrium => dissipation == production
    // Eq. 4.14, p.82
    volScalarField K1{"K1", 2.0*(1.0 + e_)*rho*gs0_};
    volScalarField K3
    {
      "K3",
      0.5*da*rho*
      (
        (sqrtPi/(3.0*(3.0 - e_)))
       *(1.0 + 0.4*(1.0 + e_)*(3.0*e_ - 1.0)*alpha*gs0_)
       +1.6*alpha*gs0_*(1.0 + e_)/sqrtPi
      )
    };
    volScalarField K2
    {
      "K2",
      4.0*da*rho*(1.0 + e_)*alpha*gs0_/(3.0*sqrtPi) - 2.0*K3/3.0
    };
    volScalarField K4{"K4", 12.0*(1.0 - sqr(e_))*rho*gs0_/(da*sqrtPi)};
    volScalarField trD
    {
      "trD",
      alpha/(alpha + residualAlpha_)
     *fvc::div(phi_)
    };
    volScalarField tr2D{"tr2D", sqr(trD)};
    volScalarField trD2{"trD2", tr(D & D)};
    volScalarField t1{"t1", K1*alpha + rho};
    volScalarField l1{"l1", -t1*trD};
    volScalarField l2{"l2", sqr(t1)*tr2D};
    volScalarField l3
    {
      "l3",
      4.0*K4*alpha*(2.0*K3*trD2 + K2*tr2D)
    };
    Theta_ = sqr
    (
      (l1 + sqrt(l2 + l3))/(2.0*max(alpha, residualAlpha_)*K4)
    );
    kappa_ = conductivityModel_->kappa(alpha, Theta_, gs0_, rho, da, e_);
  }
  Theta_.max(0);
  Theta_.min(100);

  {
    // particle viscosity (Table 3.2, p.47)
    nut_ = viscosityModel_->nu(alpha, Theta_, gs0_, rho, da, e_);
    volScalarField ThetaSqrt("sqrtTheta", sqrt(Theta_));
    // Bulk viscosity  p. 45 (Lun et al. 1984).
    lambda_ = (4.0/3.0)*sqr(alpha)*da*gs0_*(1.0 + e_)*ThetaSqrt/sqrtPi;
    // Frictional pressure
    volScalarField pf
    {
      frictionalStressModel_->frictionalPressure
      (
        alpha,
        alphaMinFriction_,
        alphaMax_
      )
    };
    // Add frictional shear viscosity, Eq. 3.30, p. 52
    nut_ += frictionalStressModel_->nu
    (
      alpha,
      alphaMinFriction_,
      alphaMax_,
      pf/rho,
      D
    );
    // Limit viscosity
    nut_.min(100);
  }
  if (debug)
  {
    Info << typeName << ':' << nl
      << "    max(Theta) = " << max(Theta_).value() << nl
      << "    max(nut) = " << max(nut_).value() << endl;
  }
}
