// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_model.hpp"
#include "diameter_model.hpp"
#include "fixed_value_fv_patch_fields.hpp"
#include "slip_fv_patch_fields.hpp"
#include "partial_slip_fv_patch_fields.hpp"
#include "surface_interpolate.hpp"
// Constructors 
mousse::phaseModel::phaseModel
(
  const word& phaseName,
  const dictionary& phaseDict,
  const fvMesh& mesh
)
:
  volScalarField
  {
    {
      IOobject::groupName("alpha", phaseName),
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  },
  name_{phaseName},
  phaseDict_{phaseDict},
  nu_
  {
    "nu",
    {0, 2, -1, 0, 0},
    phaseDict_
  },
  kappa_
  {
    "kappa",
    {1, 1, -3, -1, 0},
    phaseDict_
  },
  Cp_
  {
    "Cp",
    {0, 2, -2, -1, 0},
    phaseDict_
  },
  rho_
  {
    "rho",
    dimDensity,
    phaseDict_
  },
  U_
  {
    {
      IOobject::groupName("U", phaseName),
      mesh.time().timeName(),
      mesh,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh
  },
  DDtU_
  {
    {
      IOobject::groupName("DDtU", phaseName),
      mesh.time().timeName(),
      mesh
    },
    mesh,
    {"0", dimVelocity/dimTime, vector::zero}
  },
  alphaPhi_
  {
    {
      IOobject::groupName("alphaPhi", phaseName),
      mesh.time().timeName(),
      mesh
    },
    mesh,
    {"0", {0, 3, -1, 0, 0}, 0}
  }
{
  const word phiName = IOobject::groupName("phi", name_);
  IOobject phiHeader
  {
    phiName,
    mesh.time().timeName(),
    mesh,
    IOobject::NO_READ
  };
  if (phiHeader.headerOk())
  {
    Info<< "Reading face flux field " << phiName << endl;
    phiPtr_.reset
    (
      new surfaceScalarField
      {
        {
          phiName,
          mesh.time().timeName(),
          mesh,
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE
        },
        mesh
      }
    );
  }
  else
  {
    Info << "Calculating face flux field " << phiName << endl;
    wordList phiTypes
    {
      U_.boundaryField().size(),
      calculatedFvPatchScalarField::typeName
    };
    FOR_ALL(U_.boundaryField(), i)
    {
      if (isA<fixedValueFvPatchVectorField>(U_.boundaryField()[i])
          || isA<slipFvPatchVectorField>(U_.boundaryField()[i])
          || isA<partialSlipFvPatchVectorField>(U_.boundaryField()[i]))
      {
        phiTypes[i] = fixedValueFvPatchScalarField::typeName;
      }
    }
    phiPtr_.reset
    (
      new surfaceScalarField
      {
        {
          phiName,
          mesh.time().timeName(),
          mesh,
          IOobject::NO_READ,
          IOobject::AUTO_WRITE
        },
        fvc::interpolate(U_) & mesh.Sf(),
        phiTypes
      }
    );
  }
  dPtr_ = diameterModel::New
  (
    phaseDict_,
    *this
  );
}
// Destructor 
mousse::phaseModel::~phaseModel()
{}
// Member Functions 
mousse::autoPtr<mousse::phaseModel> mousse::phaseModel::clone() const
{
  NOT_IMPLEMENTED("phaseModel::clone() const");
  return autoPtr<phaseModel>{NULL};
}
void mousse::phaseModel::correct()
{
  //nuModel_->correct();
}
bool mousse::phaseModel::read(const dictionary& phaseDict)
{
  phaseDict_ = phaseDict;
  //if (nuModel_->read(phaseDict_))
  {
    phaseDict_.lookup("nu") >> nu_.value();
    phaseDict_.lookup("kappa") >> kappa_.value();
    phaseDict_.lookup("Cp") >> Cp_.value();
    phaseDict_.lookup("rho") >> rho_.value();
    return true;
  }
  // else
  // {
  //     return false;
  // }
  return true;
}
mousse::tmp<mousse::volScalarField> mousse::phaseModel::d() const
{
  return dPtr_().d();
}
