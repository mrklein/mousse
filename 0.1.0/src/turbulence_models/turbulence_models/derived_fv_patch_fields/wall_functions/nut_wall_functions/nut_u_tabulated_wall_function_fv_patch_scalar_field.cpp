// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nut_u_tabulated_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Protected Member Functions 
tmp<scalarField> nutUTabulatedWallFunctionFvPatchScalarField::calcNut() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
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
  const scalarField magGradU(mag(Uw.snGrad()));
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return
    max
    (
      scalar(0),
      sqr(magUp/(calcUPlus(magUp*y/nuw) + ROOTVSMALL))
     /(magGradU + ROOTVSMALL)
     - nuw
    );
}
tmp<scalarField> nutUTabulatedWallFunctionFvPatchScalarField::calcUPlus
(
  const scalarField& Rey
) const
{
  tmp<scalarField> tuPlus(new scalarField(patch().size(), 0.0));
  scalarField& uPlus = tuPlus();
  FOR_ALL(uPlus, faceI)
  {
    uPlus[faceI] = uPlusTable_.interpolateLog10(Rey[faceI]);
  }
  return tuPlus;
}
// Constructors 
nutUTabulatedWallFunctionFvPatchScalarField::
nutUTabulatedWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{p, iF},
  uPlusTableName_{"undefined-uPlusTableName"},
  uPlusTable_
  {
    // IOobject
    {
      uPlusTableName_,
      patch().boundaryMesh().mesh().time().constant(),
      patch().boundaryMesh().mesh(),
      IOobject::NO_READ,
      IOobject::NO_WRITE,
      false
    },
    false
  }
{}
nutUTabulatedWallFunctionFvPatchScalarField::
nutUTabulatedWallFunctionFvPatchScalarField
(
  const nutUTabulatedWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutWallFunctionFvPatchScalarField{ptf, p, iF, mapper},
  uPlusTableName_{ptf.uPlusTableName_},
  uPlusTable_{ptf.uPlusTable_}
{}
nutUTabulatedWallFunctionFvPatchScalarField::
nutUTabulatedWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutWallFunctionFvPatchScalarField{p, iF, dict},
  uPlusTableName_{dict.lookup("uPlusTable")},
  uPlusTable_
  {
    // IOobject
    {
      uPlusTableName_,
      patch().boundaryMesh().mesh().time().constant(),
      patch().boundaryMesh().mesh(),
      IOobject::MUST_READ_IF_MODIFIED,
      IOobject::NO_WRITE,
      false
    },
    true
  }
{}
nutUTabulatedWallFunctionFvPatchScalarField::
nutUTabulatedWallFunctionFvPatchScalarField
(
  const nutUTabulatedWallFunctionFvPatchScalarField& wfpsf
)
:
  nutWallFunctionFvPatchScalarField{wfpsf},
  uPlusTableName_{wfpsf.uPlusTableName_},
  uPlusTable_{wfpsf.uPlusTable_}
{}
nutUTabulatedWallFunctionFvPatchScalarField::
nutUTabulatedWallFunctionFvPatchScalarField
(
  const nutUTabulatedWallFunctionFvPatchScalarField& wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{wfpsf, iF},
  uPlusTableName_{wfpsf.uPlusTableName_},
  uPlusTable_{wfpsf.uPlusTable_}
{}
// Member Functions 
tmp<scalarField> nutUTabulatedWallFunctionFvPatchScalarField::yPlus() const
{
  const label patchi = patch().index();
  const turbulenceModel& turbModel = db().lookupObject<turbulenceModel>
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
  const scalarField Rey(magUp*y/nuw);
  return Rey/(calcUPlus(Rey) + ROOTVSMALL);
}
void nutUTabulatedWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  os.writeKeyword("uPlusTable") << uPlusTableName_
    << token::END_STATEMENT << nl;
  writeEntry("value", os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutUTabulatedWallFunctionFvPatchScalarField
);
}  // namespace mousse
