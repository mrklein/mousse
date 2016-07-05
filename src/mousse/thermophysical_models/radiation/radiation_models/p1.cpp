// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "p1.hpp"
#include "fvm_laplacian.hpp"
#include "fvm_sup.hpp"
#include "absorption_emission_model.hpp"
#include "scatter_model.hpp"
#include "constants.hpp"
#include "add_to_run_time_selection_table.hpp"


using namespace mousse::constant;

// Static Data Members
namespace mousse {
namespace radiation {

DEFINE_TYPE_NAME_AND_DEBUG(P1, 0);
ADD_TO_RADIATION_RUN_TIME_SELECTION_TABLES(P1);

}
}


// Constructors 
mousse::radiation::P1::P1(const volScalarField& T)
:
  radiationModel{typeName, T},
  G_
  {
    {
      "G",
      mesh_.time().timeName(),
      mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh_
  },
  Qr_
  {
    {
      "Qr",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qr", dimMass/pow3(dimTime), 0.0}
  },
  a_
  {
    {
      "a",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"a", dimless/dimLength, 0.0}
  },
  e_
  {
    {
      "e",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"e", dimless/dimLength, 0.0}
  },
  E_
  {
    {
      "E",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"E", dimMass/dimLength/pow3(dimTime), 0.0}
  }
{}


mousse::radiation::P1::P1(const dictionary& dict, const volScalarField& T)
:
  radiationModel{typeName, dict, T},
  G_
  {
    {
      "G",
      mesh_.time().timeName(),
      mesh_,
      IOobject::MUST_READ,
      IOobject::AUTO_WRITE
    },
    mesh_
  },
  Qr_
  {
    {
      "Qr",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"Qr", dimMass/pow3(dimTime), 0.0}
  },
  a_
  {
    {
      "a",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::AUTO_WRITE
    },
    mesh_,
    {"a", dimless/dimLength, 0.0}
  },
  e_
  {
    {
      "e",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"e", dimless/dimLength, 0.0}
  },
  E_
  {
    {
      "E",
      mesh_.time().timeName(),
      mesh_,
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    mesh_,
    {"E", dimMass/dimLength/pow3(dimTime), 0.0}
  }
{}


// Destructor 
mousse::radiation::P1::~P1()
{}


// Member Functions 
bool mousse::radiation::P1::read()
{
  if (radiationModel::read()) {
    // nothing to read
    return true;
  } else {
    return false;
  }
}


void mousse::radiation::P1::calculate()
{
  a_ = absorptionEmission_->a();
  e_ = absorptionEmission_->e();
  E_ = absorptionEmission_->E();
  const volScalarField sigmaEff{scatter_->sigmaEff()};
  const dimensionedScalar a0{"a0", a_.dimensions(), ROOTVSMALL};
  // Construct diffusion
  const volScalarField gamma
  {
    {
      "gammaRad",
      G_.mesh().time().timeName(),
      G_.mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE
    },
    1.0/(3.0*a_ + sigmaEff + a0)
  };
  // Solve G transport equation
  solve
  (
    fvm::laplacian(gamma, G_)
    - fvm::Sp(a_, G_)
    ==
    - 4.0*(e_*physicoChemical::sigma*pow4(T_)) - E_
  );
  // Calculate radiative heat flux on boundaries.
  FOR_ALL(mesh_.boundaryMesh(), patchi) {
    if (!G_.boundaryField()[patchi].coupled()) {
      Qr_.boundaryField()[patchi] =
        -gamma.boundaryField()[patchi]*G_.boundaryField()[patchi].snGrad();
    }
  }
}


mousse::tmp<mousse::volScalarField> mousse::radiation::P1::Rp() const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        {
          "Rp",
          mesh_.time().timeName(),
          mesh_,
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        4.0*absorptionEmission_->eCont()*physicoChemical::sigma
      }
    };
}


mousse::tmp<mousse::DimensionedField<mousse::scalar, mousse::volMesh>>
mousse::radiation::P1::Ru() const
{
  const DimensionedField<scalar, volMesh>& G = G_.dimensionedInternalField();
  const DimensionedField<scalar, volMesh> E =
    absorptionEmission_->ECont()().dimensionedInternalField();
  const DimensionedField<scalar, volMesh> a =
    absorptionEmission_->aCont()().dimensionedInternalField();
  return a*G - E;
}

