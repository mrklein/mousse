// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_model.hpp"
#include "two_phase_system.hpp"
#include "diameter_model.hpp"
#include "fv_matrix.hpp"
#include "phase_compressible_turbulence_model.hpp"
#include "drag_model.hpp"
#include "heat_transfer_model.hpp"
#include "fixed_value_fvs_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "slip_fv_patch_fields.hpp"
#include "partial_slip_fv_patch_fields.hpp"
#include "surface_interpolate.hpp"
// Constructors 
mousse::phaseModel::phaseModel
(
  const twoPhaseSystem& fluid,
  const dictionary& phaseProperties,
  const word& phaseName
)
:
  volScalarField
  {
    {
      IOobject::groupName("alpha", phaseName),
      fluid.mesh().time().timeName(),
      fluid.mesh(),
      IOobject::READ_IF_PRESENT,
      IOobject::AUTO_WRITE
    },
    fluid.mesh(),
    {"alpha", dimless, 0}
  },
  fluid_{fluid},
  name_{phaseName},
  phaseDict_{phaseProperties.subDict(name_)},
  residualAlpha_
  {
    "residualAlpha",
    dimless,
    fluid.subDict(phaseName).lookup("residualAlpha")
  },
  alphaMax_{phaseDict_.lookupOrDefault("alphaMax", 1.0)},
  thermo_{rhoThermo::New(fluid.mesh(), name_)},
  U_
  {
    {
      IOobject::groupName("U", name_),
      fluid.mesh().time().timeName(),
      fluid.mesh(),
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    fluid.mesh()
  },
  alphaPhi_
  {
    {
      IOobject::groupName("alphaPhi", name_),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", dimensionSet(0, 3, -1, 0, 0), 0}
  },
  alphaRhoPhi_
  {
    {
      IOobject::groupName("alphaRhoPhi", name_),
      fluid.mesh().time().timeName(),
      fluid.mesh()
    },
    fluid.mesh(),
    {"0", {1, 0, -1, 0, 0}, 0}
  }
{
  thermo_->validate("phaseModel " + name_, "h", "e");
  const word phiName = IOobject::groupName("phi", name_);
  IOobject phiHeader
  (
    phiName,
    fluid_.mesh().time().timeName(),
    fluid_.mesh(),
    IOobject::NO_READ
  );
  if (phiHeader.headerOk())
  {
    Info<< "Reading face flux field " << phiName << endl;
    phiPtr_.reset
    (
      new surfaceScalarField
      {
        {
          phiName,
          fluid_.mesh().time().timeName(),
          fluid_.mesh(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        },
        fluid_.mesh()
      }
    );
  }
  else
  {
    Info << "Calculating face flux field " << phiName << endl;
    wordList phiTypes
    (
      U_.boundaryField().size(),
      calculatedFvPatchScalarField::typeName
    );
    FOR_ALL(U_.boundaryField(), i)
    {
      if (isA<fixedValueFvPatchVectorField>(U_.boundaryField()[i])
          || isA<slipFvPatchVectorField>(U_.boundaryField()[i])
          || isA<partialSlipFvPatchVectorField>(U_.boundaryField()[i]))
      {
        phiTypes[i] = fixedValueFvsPatchScalarField::typeName;
      }
    }
    phiPtr_.reset
    (
      new surfaceScalarField
      {
        {
          phiName,
          fluid_.mesh().time().timeName(),
          fluid_.mesh(),
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        fvc::interpolate(U_) & fluid_.mesh().Sf(),
        phiTypes
      }
    );
  }
  dPtr_ = diameterModel::New
  (
    phaseDict_,
    *this
  );
  turbulence_ =
    PhaseCompressibleTurbulenceModel<phaseModel>::New
    (
      *this,
      thermo_->rho(),
      U_,
      alphaRhoPhi_,
      phi(),
      *this
    );
}
// Destructor 
mousse::phaseModel::~phaseModel()
{}
// Member Functions 
const mousse::phaseModel& mousse::phaseModel::otherPhase() const
{
  return fluid_.otherPhase(*this);
}
mousse::tmp<mousse::volScalarField> mousse::phaseModel::d() const
{
  return dPtr_().d();
}
mousse::PhaseCompressibleTurbulenceModel<mousse::phaseModel>&
mousse::phaseModel::turbulence()
{
  return turbulence_();
}
const mousse::PhaseCompressibleTurbulenceModel<mousse::phaseModel>&
mousse::phaseModel::turbulence() const
{
  return turbulence_();
}
void mousse::phaseModel::correct()
{
  return dPtr_->correct();
}
bool mousse::phaseModel::read(const dictionary& phaseProperties)
{
  phaseDict_ = phaseProperties.subDict(name_);
  return dPtr_->read(phaseDict_);
}
