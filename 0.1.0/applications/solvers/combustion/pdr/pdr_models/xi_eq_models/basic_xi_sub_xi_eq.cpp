// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "basic_xi_sub_xi_eq.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Static Data Members
namespace mousse
{
namespace XiEqModels
{
DEFINE_TYPE_NAME_AND_DEBUG(basicSubGrid, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(XiEqModel, basicSubGrid, dictionary);
}
}
// Constructors 
mousse::XiEqModels::basicSubGrid::basicSubGrid
(
  const dictionary& XiEqProperties,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
:
  XiEqModel{XiEqProperties, thermo, turbulence, Su},
  B_
  {
    {
      "B",
      Su.mesh().facesInstance(),
      Su.mesh(),
      IOobject::MUST_READ
    },
    Su.mesh()
  },
  XiEqModel_{XiEqModel::New(XiEqModelCoeffs_, thermo, turbulence, Su)}
{}
// Destructor 
mousse::XiEqModels::basicSubGrid::~basicSubGrid()
{}
// Member Functions 
mousse::tmp<mousse::volScalarField> mousse::XiEqModels::basicSubGrid::XiEq() const
{
  const fvMesh& mesh = Su_.mesh();
  const volVectorField& U = mesh.lookupObject<volVectorField>("U");
  const volScalarField& Nv = mesh.lookupObject<volScalarField>("Nv");
  const volSymmTensorField& nsv =
    mesh.lookupObject<volSymmTensorField>("nsv");
  volScalarField magU{mag(U)};
  volVectorField Uhat
  {
    U/(mag(U) + dimensionedScalar{"Usmall", U.dimensions(), 1e-4})
  };
  const scalarField Cw = pow(mesh.V(), 2.0/3.0);
  tmp<volScalarField> tN
  {
    new volScalarField
    {
      {
        "tN",
        mesh.time().constant(),
        mesh
      },
      mesh,
      {"zero", Nv.dimensions(), 0.0},
      zeroGradientFvPatchVectorField::typeName
    }
  };
  volScalarField& N = tN();
  N.internalField() = Nv.internalField()*Cw;
  tmp<volSymmTensorField> tns
  {
    new volSymmTensorField
    {
      {
        "tns",
        U.mesh().time().timeName(),
        U.mesh()
      },
      U.mesh(),
      {"zero", nsv.dimensions(), pTraits<symmTensor>::zero},
      zeroGradientFvPatchSymmTensorField::typeName
    }
  };
  volSymmTensorField& ns = tns();
  ns.internalField() = nsv.internalField()*Cw;
  volScalarField n{max(N - (Uhat & ns & Uhat), scalar(1e-4))};
  volScalarField b{(Uhat & B_ & Uhat)/sqrt(n)};
  volScalarField up{sqrt((2.0/3.0)*turbulence_.k())};
  volScalarField XiSubEq
  {
    scalar(1)
    + max(2.2*sqrt(b), min(0.34*magU/up*sqrt(b), scalar(1.6)))
    *min(n, scalar(1))
  };
  return XiSubEq*XiEqModel_->XiEq();
}
bool mousse::XiEqModels::basicSubGrid::read(const dictionary& XiEqProperties)
{
  XiEqModel::read(XiEqProperties);
  return XiEqModel_->read(XiEqModelCoeffs_);
}
