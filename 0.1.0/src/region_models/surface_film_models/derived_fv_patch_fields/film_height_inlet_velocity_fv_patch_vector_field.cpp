// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "film_height_inlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


// Constructors 
mousse::filmHeightInletVelocityFvPatchVectorField::
filmHeightInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"},
  deltafName_{"deltaf"}
{}


mousse::filmHeightInletVelocityFvPatchVectorField::
filmHeightInletVelocityFvPatchVectorField
(
  const filmHeightInletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  deltafName_{ptf.deltafName_}
{}


mousse::filmHeightInletVelocityFvPatchVectorField::
filmHeightInletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  deltafName_{dict.lookupOrDefault<word>("deltaf", "deltaf")}
{
  fvPatchVectorField::operator=(vectorField{"value", dict, p.size()});
}


mousse::filmHeightInletVelocityFvPatchVectorField::
filmHeightInletVelocityFvPatchVectorField
(
  const filmHeightInletVelocityFvPatchVectorField& fhivpvf
)
:
  fixedValueFvPatchVectorField{fhivpvf},
  phiName_{fhivpvf.phiName_},
  rhoName_{fhivpvf.rhoName_},
  deltafName_{fhivpvf.deltafName_}
{}


mousse::filmHeightInletVelocityFvPatchVectorField::
filmHeightInletVelocityFvPatchVectorField
(
  const filmHeightInletVelocityFvPatchVectorField& fhivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{fhivpvf, iF},
  phiName_{fhivpvf.phiName_},
  rhoName_{fhivpvf.rhoName_},
  deltafName_{fhivpvf.deltafName_}
{}


// Member Functions 
void mousse::filmHeightInletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  const fvPatchField<scalar>& rhop =
    patch().lookupPatchField<volScalarField, scalar>(rhoName_);
  const fvPatchField<scalar>& deltafp =
    patch().lookupPatchField<volScalarField, scalar>(deltafName_);
  vectorField n{patch().nf()};
  const scalarField& magSf = patch().magSf();
  operator==(n*phip/(rhop*magSf*deltafp + ROOTVSMALL));
  fixedValueFvPatchVectorField::updateCoeffs();
}


void mousse::filmHeightInletVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  writeEntryIfDifferent<word>(os, "deltaf", "deltaf", deltafName_);
  writeEntry("value", os);
}


// Member Operators 
void mousse::filmHeightInletVelocityFvPatchVectorField::operator=
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
  filmHeightInletVelocityFvPatchVectorField
);

}

