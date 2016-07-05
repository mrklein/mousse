// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_eq_model.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(XiEqModel, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(XiEqModel, dictionary);

}


// Constructors 
mousse::XiEqModel::XiEqModel
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModelCoeffs_
  {
    XiEqProperties.subDict
    (
      word(XiEqProperties.lookup("XiEqModel")) + "Coeffs"
    )
  },
  thermo_{thermo},
  turbulence_{turbulence},
  Su_{Su}
{}


// Destructor 
mousse::XiEqModel::~XiEqModel()
{}


// Member Functions 
bool mousse::XiEqModel::read(const dictionary& XiEqProperties)
{
  XiEqModelCoeffs_ = XiEqProperties.subDict(type() + "Coeffs");
  return true;
}


void mousse::XiEqModel::writeFields() const
{
  //***HGW It is not clear why B is written here
  if (Su_.mesh().foundObject<volSymmTensorField>("B")) {
    const volSymmTensorField& B =
      Su_.mesh().lookupObject<volSymmTensorField>("B");
    B.write();
  }
}


mousse::tmp<mousse::volScalarField>
mousse::XiEqModel::calculateSchelkinEffect(const scalar uPrimeCoef) const
{
  const fvMesh& mesh = Su_.mesh();
  const volVectorField& U = mesh.lookupObject<volVectorField>("U");
  const volSymmTensorField& CT = mesh.lookupObject<volSymmTensorField>("CT");
  const volScalarField& Nv = mesh.lookupObject<volScalarField>("Nv");
  const volSymmTensorField& nsv = mesh.lookupObject<volSymmTensorField>("nsv");
  tmp<volScalarField> tN
  {
    new volScalarField
    {
      {
        "tN",
        mesh.time().timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      mesh,
      {"zero", Nv.dimensions(), 0.0},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  volScalarField& N = tN();
  N.internalField() = Nv.internalField()*pow(mesh.V(), 2.0/3.0);
  tmp<volSymmTensorField> tns
  {
    new volSymmTensorField
    {
      {
        "tns",
        mesh.time().timeName(),
        mesh
      },
      mesh,
      {"zero", nsv.dimensions(), pTraits<symmTensor>::zero}
    }
  };
  volSymmTensorField& ns = tns();
  ns.internalField() = nsv.internalField()*pow(mesh.V(), 2.0/3.0);
  const volVectorField Uhat
  {
    U/(mag(U) + dimensionedScalar("Usmall", U.dimensions(), 1e-4))
  };
  const volScalarField nr{sqrt(max(N - (Uhat & ns & Uhat), scalar(1e-4)))};
  const scalarField cellWidth{pow(mesh.V(), 1.0/3.0)};
  const scalarField upLocal{uPrimeCoef*sqrt((U & CT & U)*cellWidth)};
  const scalarField deltaUp{upLocal*(max(scalar(1.0), pow(nr, 0.5)) - 1.0)};
  // Re use tN
  N.internalField() = upLocal*(max(scalar(1.0), pow(nr, 0.5)) - 1.0);
  return tN;
}

