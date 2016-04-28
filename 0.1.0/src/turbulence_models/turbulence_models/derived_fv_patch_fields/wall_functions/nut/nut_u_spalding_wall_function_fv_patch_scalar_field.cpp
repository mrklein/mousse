// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nut_u_spalding_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Protected Member Functions 
tmp<scalarField> nutUSpaldingWallFunctionFvPatchScalarField::calcNut() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField magGradU{mag(Uw.snGrad())};
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return max(scalar(0), sqr(calcUTau(magGradU))/(magGradU + ROOTVSMALL) - nuw);
}


tmp<scalarField> nutUSpaldingWallFunctionFvPatchScalarField::calcUTau
(
  const scalarField& magGradU
) const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField& y = turbModel.y()[patchi];
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const scalarField magUp(mag(Uw.patchInternalField() - Uw));
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const scalarField& nutw = *this;
  tmp<scalarField> tuTau{new scalarField{patch().size(), 0.0}};
  scalarField& uTau = tuTau();
  FOR_ALL(uTau, faceI) {
    scalar ut = sqrt((nutw[faceI] + nuw[faceI])*magGradU[faceI]);
    if (ut > ROOTVSMALL) {
      int iter = 0;
      scalar err = GREAT;
      do {
        scalar kUu = min(kappa_*magUp[faceI]/ut, 50);
        scalar fkUu = exp(kUu) - 1 - kUu*(1 + 0.5*kUu);
        scalar f = -ut*y[faceI]/nuw[faceI] + magUp[faceI]/ut
          + 1/E_*(fkUu - 1.0/6.0*kUu*sqr(kUu));
        scalar df = y[faceI]/nuw[faceI] + magUp[faceI]/sqr(ut)
          + 1/E_*kUu*fkUu/ut;
        scalar uTauNew = ut + f/df;
        err = mag((ut - uTauNew)/ut);
        ut = uTauNew;
      } while (ut > ROOTVSMALL && err > 0.01 && ++iter < 10);
      uTau[faceI] = max(0.0, ut);
    }
  }
  return tuTau;
}


// Constructors 
nutUSpaldingWallFunctionFvPatchScalarField::
nutUSpaldingWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{p, iF}
{}


nutUSpaldingWallFunctionFvPatchScalarField::
nutUSpaldingWallFunctionFvPatchScalarField
(
  const nutUSpaldingWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutWallFunctionFvPatchScalarField{ptf, p, iF, mapper}
{}


nutUSpaldingWallFunctionFvPatchScalarField::
nutUSpaldingWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutWallFunctionFvPatchScalarField{p, iF, dict}
{}


nutUSpaldingWallFunctionFvPatchScalarField::
nutUSpaldingWallFunctionFvPatchScalarField
(
  const nutUSpaldingWallFunctionFvPatchScalarField& wfpsf
)
:
  nutWallFunctionFvPatchScalarField{wfpsf}
{}


nutUSpaldingWallFunctionFvPatchScalarField::
nutUSpaldingWallFunctionFvPatchScalarField
(
  const nutUSpaldingWallFunctionFvPatchScalarField& wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{wfpsf, iF}
{}


// Member Functions 
tmp<scalarField> nutUSpaldingWallFunctionFvPatchScalarField::yPlus() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel =
    db().lookupObject<turbulenceModel>
    (
      IOobject::groupName
      (
        turbulenceModel::propertiesName,
        dimensionedInternalField().group()
      )
    );
  const scalarField& y = turbModel.y()[patchi];
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return y*calcUTau(mag(Uw.snGrad()))/nuw;
}


void nutUSpaldingWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  writeLocalEntries(os);
  writeEntry("value", os);
}


MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutUSpaldingWallFunctionFvPatchScalarField
);

}  // namespace mousse

