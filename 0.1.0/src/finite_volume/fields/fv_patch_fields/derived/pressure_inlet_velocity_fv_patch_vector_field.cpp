// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_inlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


// Constructors 
mousse::pressureInletVelocityFvPatchVectorField::
pressureInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"}
{}


mousse::pressureInletVelocityFvPatchVectorField::
pressureInletVelocityFvPatchVectorField
(
  const pressureInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_}
{}


mousse::pressureInletVelocityFvPatchVectorField::
pressureInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}


mousse::pressureInletVelocityFvPatchVectorField::
pressureInletVelocityFvPatchVectorField
(
  const pressureInletVelocityFvPatchVectorField& pivpvf
)
:
  fixedValueFvPatchVectorField{pivpvf},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_}
{}


mousse::pressureInletVelocityFvPatchVectorField::
pressureInletVelocityFvPatchVectorField
(
  const pressureInletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{pivpvf, iF},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_}
{}


// Member Functions 
void mousse::pressureInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  tmp<vectorField> n = patch().nf();
  const Field<scalar>& magS = patch().magSf();
  if (phi.dimensions() == dimVelocity*dimArea) {
    operator==(n*phip/magS);
  } else if (phi.dimensions() == dimDensity*dimVelocity*dimArea) {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    operator==(n*phip/(rhop*magS));
  } else {
    FATAL_ERROR_IN("pressureInletVelocityFvPatchVectorField::updateCoeffs()")
      << "dimensions of phi are not correct"
      << "\n    on patch " << this->patch().name()
      << " of field " << this->dimensionedInternalField().name()
      << " in file " << this->dimensionedInternalField().objectPath()
      << exit(FatalError);
  }
  fixedValueFvPatchVectorField::updateCoeffs();
}


void mousse::pressureInletVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntry("value", os);
}


// Member Operators 
void mousse::pressureInletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  fvPatchField<vector>::operator=(patch().nf()*(patch().nf() & pvf));
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureInletVelocityFvPatchVectorField
);

}

