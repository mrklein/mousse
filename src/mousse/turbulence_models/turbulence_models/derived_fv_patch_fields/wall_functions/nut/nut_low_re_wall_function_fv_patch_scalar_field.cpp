// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nut_low_re_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Protected Member Functions 
tmp<scalarField> nutLowReWallFunctionFvPatchScalarField::calcNut() const
{
  return tmp<scalarField>{new scalarField{patch().size(), 0.0}};
}


// Constructors 
nutLowReWallFunctionFvPatchScalarField::nutLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{p, iF}
{}


nutLowReWallFunctionFvPatchScalarField::nutLowReWallFunctionFvPatchScalarField
(
  const nutLowReWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutWallFunctionFvPatchScalarField{ptf, p, iF, mapper}
{}


nutLowReWallFunctionFvPatchScalarField::nutLowReWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutWallFunctionFvPatchScalarField{p, iF, dict}
{}


nutLowReWallFunctionFvPatchScalarField::nutLowReWallFunctionFvPatchScalarField
(
  const nutLowReWallFunctionFvPatchScalarField& nlrwfpsf
)
:
  nutWallFunctionFvPatchScalarField{nlrwfpsf}
{}


nutLowReWallFunctionFvPatchScalarField::nutLowReWallFunctionFvPatchScalarField
(
  const nutLowReWallFunctionFvPatchScalarField& nlrwfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{nlrwfpsf, iF}
{}


// Member Functions 
tmp<scalarField> nutLowReWallFunctionFvPatchScalarField::yPlus() const
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
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const fvPatchVectorField& Uw = turbModel.U().boundaryField()[patchi];
  return y*sqrt(nuw*mag(Uw.snGrad()))/nuw;
}

MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutLowReWallFunctionFvPatchScalarField
);

}  // namespace mousse

