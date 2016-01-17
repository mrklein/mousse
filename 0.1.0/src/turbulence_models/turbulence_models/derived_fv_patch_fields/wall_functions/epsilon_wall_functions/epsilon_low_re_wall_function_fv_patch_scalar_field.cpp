// mousse: CFD toolbox
// Copyright (C) 2012-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "epsilon_low_re_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
// Protected Member Functions 
mousse::scalar mousse::epsilonLowReWallFunctionFvPatchScalarField::yPlusLam
(
  const scalar kappa,
  const scalar E
)
{
  scalar ypl = 11.0;
  for (int i=0; i<10; i++)
  {
    ypl = log(max(E*ypl, 1))/kappa;
  }
  return ypl;
}
void mousse::epsilonLowReWallFunctionFvPatchScalarField::calculate
(
  const turbulenceModel& turbModel,
  const List<scalar>& cornerWeights,
  const fvPatch& patch,
  scalarField& G0,
  scalarField& epsilon0
)
{
  const label patchi = patch.index();
  const scalarField& y = turbModel.y()[patchi];
  const scalar Cmu25 = pow025(Cmu_);
  const scalar Cmu75 = pow(Cmu_, 0.75);
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const tmp<scalarField> tnutw = turbModel.nut(patchi);
  const scalarField& nutw = tnutw();
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField magGradUw(mag(Uw.snGrad()));
  const DimensionedField<scalar, volMesh>& G =
    db().lookupObject<DimensionedField<scalar, volMesh> >
    (
      turbModel.GName()
    );
  // Set epsilon and G
  FOR_ALL(nutw, facei)
  {
    label celli = patch.faceCells()[facei];
    scalar yPlus = Cmu25*sqrt(k[celli])*y[facei]/nuw[facei];
    scalar w = cornerWeights[facei];
    if (yPlus > yPlusLam_)
    {
      epsilon0[celli] += w*Cmu75*pow(k[celli], 1.5)/(kappa_*y[facei]);
      G0[celli] +=
        w
       *(nutw[facei] + nuw[facei])
       *magGradUw[facei]
       *Cmu25*sqrt(k[celli])
       /(kappa_*y[facei]);
    }
    else
    {
      epsilon0[celli] += w*2.0*k[celli]*nuw[facei]/sqr(y[facei]);
      G0[celli] += G[celli];
    }
  }
}
// Constructors 
mousse::epsilonLowReWallFunctionFvPatchScalarField::
epsilonLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  epsilonWallFunctionFvPatchScalarField{p, iF},
  yPlusLam_{yPlusLam(kappa_, E_)}
{}
mousse::epsilonLowReWallFunctionFvPatchScalarField::
epsilonLowReWallFunctionFvPatchScalarField
(
  const epsilonLowReWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  epsilonWallFunctionFvPatchScalarField{ptf, p, iF, mapper},
  yPlusLam_{ptf.yPlusLam_}
{}
mousse::epsilonLowReWallFunctionFvPatchScalarField::
epsilonLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  epsilonWallFunctionFvPatchScalarField{p, iF, dict},
  yPlusLam_{yPlusLam(kappa_, E_)}
{}
mousse::epsilonLowReWallFunctionFvPatchScalarField::
epsilonLowReWallFunctionFvPatchScalarField
(
  const epsilonLowReWallFunctionFvPatchScalarField& ewfpsf
)
:
  epsilonWallFunctionFvPatchScalarField{ewfpsf},
  yPlusLam_{ewfpsf.yPlusLam_}
{}
mousse::epsilonLowReWallFunctionFvPatchScalarField::
epsilonLowReWallFunctionFvPatchScalarField
(
  const epsilonLowReWallFunctionFvPatchScalarField& ewfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  epsilonWallFunctionFvPatchScalarField{ewfpsf, iF},
  yPlusLam_{ewfpsf.yPlusLam_}
{}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  epsilonLowReWallFunctionFvPatchScalarField
);
}
