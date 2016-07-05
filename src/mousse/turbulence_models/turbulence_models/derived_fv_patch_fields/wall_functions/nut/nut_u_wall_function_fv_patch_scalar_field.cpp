// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "nut_u_wall_function_fv_patch_scalar_field.hpp"
#include "turbulence_model.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"


namespace mousse {

// Protected Member Functions 
tmp<scalarField> nutUWallFunctionFvPatchScalarField::calcNut() const
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
  const scalarField magUp{mag(Uw.patchInternalField() - Uw)};
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  tmp<scalarField> tyPlus = calcYPlus(magUp);
  scalarField& yPlus = tyPlus();
  tmp<scalarField> tnutw{new scalarField{patch().size(), 0.0}};
  scalarField& nutw = tnutw();
  FOR_ALL(yPlus, facei) {
    if (yPlus[facei] > yPlusLam_) {
      nutw[facei] =
        nuw[facei]*(yPlus[facei]*kappa_/log(E_*yPlus[facei]) - 1.0);
    }
  }
  return tnutw;
}


tmp<scalarField> nutUWallFunctionFvPatchScalarField::calcYPlus
(
  const scalarField& magUp
) const
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
  const tmp<scalarField> tnuw = turbModel.nu(patchi);
  const scalarField& nuw = tnuw();
  tmp<scalarField> tyPlus{new scalarField{patch().size(), 0.0}};
  scalarField& yPlus = tyPlus();
  FOR_ALL(yPlus, facei) {
    scalar kappaRe = kappa_*magUp[facei]*y[facei]/nuw[facei];
    scalar yp = yPlusLam_;
    scalar ryPlusLam = 1.0/yp;
    int iter = 0;
    scalar yPlusLast = 0.0;
    do {
      yPlusLast = yp;
      yp = (kappaRe + yp)/(1.0 + log(E_*yp));
    } while (mag(ryPlusLam*(yp - yPlusLast)) > 0.01 && ++iter < 10 );
    yPlus[facei] = max(0.0, yp);
  }
  return tyPlus;
}


// Constructors 
nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{p, iF}
{}


nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
(
  const nutUWallFunctionFvPatchScalarField& ptf,
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  nutWallFunctionFvPatchScalarField{ptf, p, iF, mapper}
{}


nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
(
  const fvPatch& p,
  const DimensionedField<scalar, volMesh>& iF,
  const dictionary& dict
)
:
  nutWallFunctionFvPatchScalarField{p, iF, dict}
{}


nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
(
  const nutUWallFunctionFvPatchScalarField& sawfpsf
)
:
  nutWallFunctionFvPatchScalarField{sawfpsf}
{}


nutUWallFunctionFvPatchScalarField::nutUWallFunctionFvPatchScalarField
(
  const nutUWallFunctionFvPatchScalarField& sawfpsf,
  const DimensionedField<scalar, volMesh>& iF
)
:
  nutWallFunctionFvPatchScalarField{sawfpsf, iF}
{}


// Member Functions 
tmp<scalarField> nutUWallFunctionFvPatchScalarField::yPlus() const
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
  const scalarField magUp{mag(Uw.patchInternalField() - Uw)};
  return calcYPlus(magUp);
}


void nutUWallFunctionFvPatchScalarField::write(Ostream& os) const
{
  fvPatchField<scalar>::write(os);
  writeLocalEntries(os);
  writeEntry("value", os);
}


MAKE_PATCH_TYPE_FIELD
(
  fvPatchScalarField,
  nutUWallFunctionFvPatchScalarField
);

}  // namespace mousse