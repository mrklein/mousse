// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nutk_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "wall_fv_patch.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
// Protected Member Functions 
tmp<scalarField> nutkWallFunctionFvPatchScalarField::calcNut() const
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
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  const scalar Cmu25 = pow025(Cmu_);
  tmp<scalarField> tnutw(new scalarField(patch().size(), 0.0));
  scalarField& nutw = tnutw();
  FOR_ALL(nutw, faceI)
  {
    label faceCellI = patch().faceCells()[faceI];
    scalar yPlus = Cmu25*y[faceI]*sqrt(k[faceCellI])/nuw[faceI];
    if (yPlus > yPlusLam_)
    {
      nutw[faceI] = nuw[faceI]*(yPlus*kappa_/log(E_*yPlus) - 1.0);
    }
  }
  return tnutw;
}
// Constructors 
nutkWallFunctionFvPatchScalarField::nutkWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{p, iF}
{}
nutkWallFunctionFvPatchScalarField::nutkWallFunctionFvPatchScalarField
(
  const nutkWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutWallFunctionFvPatchScalarField{ptf, p, iF, mapper}
{}
nutkWallFunctionFvPatchScalarField::nutkWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutWallFunctionFvPatchScalarField{p, iF, dict}
{}
nutkWallFunctionFvPatchScalarField::nutkWallFunctionFvPatchScalarField
(
  const nutkWallFunctionFvPatchScalarField& wfpsf
)
:
  nutWallFunctionFvPatchScalarField{wfpsf}
{}
nutkWallFunctionFvPatchScalarField::nutkWallFunctionFvPatchScalarField
(
  const nutkWallFunctionFvPatchScalarField& wfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{wfpsf, iF}
{}
// Member Functions 
tmp<scalarField> nutkWallFunctionFvPatchScalarField::yPlus() const
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
  const tmp<volScalarField> tk = turbModel.k();
  const volScalarField& k = tk();
  tmp<scalarField> kwc = k.boundaryField()[patchi].patchInternalField();
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  return pow025(Cmu_)*y*sqrt(kwc)/nuw;
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutkWallFunctionFvPatchScalarField
);
}  // namespace mousse
