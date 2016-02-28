// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "moving_phase_model.hpp"
#include "phase_system.hpp"
#include "euler_phase_compressible_turbulence_model.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "slip_fv_patch_fields.hpp"
#include "partial_slip_fv_patch_fields.hpp"
#include "fvm_ddt.hpp"
#include "fvm_div.hpp"
#include "fvm_sup.hpp"
#include "fvc_ddt.hpp"
#include "fvc_div.hpp"
#include "surface_interpolate.hpp"
#include "fv_matrix.hpp"
// Static Member Functions
template<class BasePhaseModel>
mousse::tmp<mousse::surfaceScalarField>
mousse::MovingPhaseModel<BasePhaseModel>::phi(const volVectorField& U) const
{
  word phiName{IOobject::groupName("phi", this->name())};
  IOobject phiHeader
  {
    phiName,
    U.mesh().time().timeName(),
    U.mesh(),
    IOobject::NO_READ
  };
  if (phiHeader.headerOk())
  {
    Info<< "Reading face flux field " << phiName << endl;
    return tmp<surfaceScalarField>
    (
      new surfaceScalarField
      (
        IOobject
        (
          phiName,
          U.mesh().time().timeName(),
          U.mesh(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        ),
        U.mesh()
      )
    );
  }
  else
  {
    Info << "Calculating face flux field " << phiName << endl;
    wordList phiTypes
    {
      U.boundaryField().size(),
      calculatedFvPatchScalarField::typeName
    };
    FOR_ALL(U.boundaryField(), i)
    {
      if (isA<fixedValueFvPatchVectorField>(U.boundaryField()[i])
          || isA<slipFvPatchVectorField>(U.boundaryField()[i])
          || isA<partialSlipFvPatchVectorField>(U.boundaryField()[i]))
      {
        phiTypes[i] = fixedValueFvPatchScalarField::typeName;
      }
    }
    return tmp<surfaceScalarField>
    {
      new surfaceScalarField
      {
        {
          phiName,
          U.mesh().time().timeName(),
          U.mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        fvc::interpolate(U) & U.mesh().Sf(),
        phiTypes
      }
    };
  }
}
// Constructors 
template<class BasePhaseModel>
mousse::MovingPhaseModel<BasePhaseModel>::MovingPhaseModel
(
  const phaseSystem& fluid,
  const word& phaseName,
  const label index
)
:
  BasePhaseModel{fluid, phaseName, index},
  U_
  {
    {
      IOobject::groupName("U", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    fluid.mesh()
  },
  phi_{phi(U_)},
  alphaPhi_
  {
    {
      IOobject::groupName("alphaPhi", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", {0, 3, -1, 0, 0}, 0}
  },
  alphaRhoPhi_
  {
    {
      IOobject::groupName("alphaRhoPhi", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", {1, 0, -1, 0, 0}, 0}
  },
  DUDt_
  {
    {
      IOobject::groupName("DUDt", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", dimAcceleration, vector::zero}
  },
  turbulence_
  {
    phaseCompressibleTurbulenceModel::New
    (
      *this,
      this->thermo().rho(),
      U_,
      alphaRhoPhi_,
      phi_,
      *this
    )
  },
  continuityError_
  {
    {
      IOobject::groupName("continuityError", this->name()),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", dimDensity/dimTime, 0}
  }
{
  phi_.writeOpt() = IOobject::AUTO_WRITE;
  correctKinematics();
}
// Destructor 
template<class BasePhaseModel>
mousse::MovingPhaseModel<BasePhaseModel>::~MovingPhaseModel()
{}
// Member Functions 
template<class BasePhaseModel>
void mousse::MovingPhaseModel<BasePhaseModel>::correct()
{
  BasePhaseModel::correct();
  this->fluid().MRF().correctBoundaryVelocity(U_);
  volScalarField& rho = this->thermo().rho();
  continuityError_ = fvc::ddt(*this, rho) + fvc::div(alphaRhoPhi_)
    - (this->fluid().fvOptions()(*this, rho)&rho);
}
template<class BasePhaseModel>
void mousse::MovingPhaseModel<BasePhaseModel>::correctKinematics()
{
  BasePhaseModel::correctKinematics();
  DUDt_ = fvc::ddt(U_) + fvc::div(phi_, U_) - fvc::div(phi_)*U_;
}
template<class BasePhaseModel>
void mousse::MovingPhaseModel<BasePhaseModel>::correctTurbulence()
{
  BasePhaseModel::correctTurbulence();
  turbulence_->correct();
}
template<class BasePhaseModel>
void mousse::MovingPhaseModel<BasePhaseModel>::correctEnergyTransport()
{
  BasePhaseModel::correctEnergyTransport();
  turbulence_->correctEnergyTransport();
}
template<class BasePhaseModel>
mousse::tmp<mousse::fvVectorMatrix>
mousse::MovingPhaseModel<BasePhaseModel>::UEqn()
{
  return
  (
    fvm::ddt(*this, this->thermo().rho(), U_)
    + fvm::div(alphaRhoPhi_, U_)
    - fvm::Sp(continuityError_, U_)
    + this->fluid().MRF().DDt(*this*this->thermo().rho(), U_)
    + turbulence_->divDevRhoReff(U_)
  );
}
template<class BasePhaseModel>
const mousse::surfaceScalarField&
mousse::MovingPhaseModel<BasePhaseModel>::DbyA() const
{
  if (DbyA_.valid())
  {
    return DbyA_;
  }
  else
  {
    return surfaceScalarField::null();
  }
}
template<class BasePhaseModel>
void mousse::MovingPhaseModel<BasePhaseModel>::DbyA
(
  const tmp<surfaceScalarField>& DbyA
)
{
  DbyA_ = DbyA;
}
template<class BasePhaseModel>
mousse::tmp<mousse::volVectorField>
mousse::MovingPhaseModel<BasePhaseModel>::U() const
{
  return U_;
}
template<class BasePhaseModel>
mousse::volVectorField&
mousse::MovingPhaseModel<BasePhaseModel>::U()
{
  return U_;
}
template<class BasePhaseModel>
mousse::tmp<mousse::volVectorField>
mousse::MovingPhaseModel<BasePhaseModel>::DUDt() const
{
  return DUDt_;
}
template<class BasePhaseModel>
mousse::tmp<mousse::volScalarField>
mousse::MovingPhaseModel<BasePhaseModel>::continuityError() const
{
  return continuityError_;
}
template<class BasePhaseModel>
mousse::tmp<mousse::surfaceScalarField>
mousse::MovingPhaseModel<BasePhaseModel>::phi() const
{
  return phi_;
}
template<class BasePhaseModel>
mousse::surfaceScalarField&
mousse::MovingPhaseModel<BasePhaseModel>::phi()
{
  return phi_;
}
template<class BasePhaseModel>
mousse::tmp<mousse::surfaceScalarField>
mousse::MovingPhaseModel<BasePhaseModel>::alphaPhi() const
{
  return alphaPhi_;
}
template<class BasePhaseModel>
mousse::surfaceScalarField&
mousse::MovingPhaseModel<BasePhaseModel>::alphaPhi()
{
  return alphaPhi_;
}
template<class BasePhaseModel>
mousse::tmp<mousse::surfaceScalarField>
mousse::MovingPhaseModel<BasePhaseModel>::alphaRhoPhi() const
{
  return alphaRhoPhi_;
}
template<class BasePhaseModel>
mousse::surfaceScalarField&
mousse::MovingPhaseModel<BasePhaseModel>::alphaRhoPhi()
{
  return alphaRhoPhi_;
}
template<class BasePhaseModel>
const mousse::phaseCompressibleTurbulenceModel&
mousse::MovingPhaseModel<BasePhaseModel>::turbulence() const
{
  return turbulence_;
}
