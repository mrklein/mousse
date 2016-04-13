// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_directed_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "fv_patch_field_mapper.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"


// Constructors 
mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::
pressureDirectedInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField{p, iF},
  phiName_{"phi"},
  rhoName_{"rho"},
  inletDir_{p.size()}
{
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}


mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::
pressureDirectedInletOutletVelocityFvPatchVectorField
(
  const pressureDirectedInletOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  mixedFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_},
  rhoName_{ptf.rhoName_},
  inletDir_{ptf.inletDir_, mapper}
{}


mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::
pressureDirectedInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  mixedFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")},
  rhoName_{dict.lookupOrDefault<word>("rho", "rho")},
  inletDir_{"inletDirection", dict, p.size()}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
  refValue() = *this;
  refGrad() = vector::zero;
  valueFraction() = 0.0;
}


mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::
pressureDirectedInletOutletVelocityFvPatchVectorField
(
  const pressureDirectedInletOutletVelocityFvPatchVectorField& pivpvf
)
:
  mixedFvPatchVectorField{pivpvf},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_},
  inletDir_{pivpvf.inletDir_}
{}


mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::
pressureDirectedInletOutletVelocityFvPatchVectorField
(
  const pressureDirectedInletOutletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  mixedFvPatchVectorField{pivpvf, iF},
  phiName_{pivpvf.phiName_},
  rhoName_{pivpvf.rhoName_},
  inletDir_{pivpvf.inletDir_}
{}


// Member Functions 
void mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  mixedFvPatchVectorField::autoMap(m);
  inletDir_.autoMap(m);
}


void mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  mixedFvPatchVectorField::rmap(ptf, addr);
  const pressureDirectedInletOutletVelocityFvPatchVectorField& tiptf =
    refCast<const pressureDirectedInletOutletVelocityFvPatchVectorField>
    (ptf);
  inletDir_.rmap(tiptf.inletDir_, addr);
}


void mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const surfaceScalarField& phi =
    db().lookupObject<surfaceScalarField>(phiName_);
  const fvsPatchField<scalar>& phip =
    patch().patchField<surfaceScalarField, scalar>(phi);
  tmp<vectorField> n = patch().nf();
  tmp<scalarField> ndmagS = (n & inletDir_)*patch().magSf();
  if (phi.dimensions() == dimVelocity*dimArea) {
    refValue() = inletDir_*phip/ndmagS;
  } else if (phi.dimensions() == dimDensity*dimVelocity*dimArea) {
    const fvPatchField<scalar>& rhop =
      patch().lookupPatchField<volScalarField, scalar>(rhoName_);
    refValue() = inletDir_*phip/(rhop*ndmagS);
  } else {
    FATAL_ERROR_IN
    (
      "pressureDirectedInletOutletVelocityFvPatchVectorField::"
      "updateCoeffs()"
    )
    << "dimensions of phi are not correct"
    << "\n    on patch " << this->patch().name()
    << " of field " << this->dimensionedInternalField().name()
    << " in file " << this->dimensionedInternalField().objectPath()
    << exit(FatalError);
  }
  valueFraction() = 1.0 - pos(phip);
  mixedFvPatchVectorField::updateCoeffs();
}


void mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::write
(
  Ostream& os
) const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
  inletDir_.writeEntry("inletDirection", os);
  writeEntry("value", os);
}


// Member Operators 
void mousse::pressureDirectedInletOutletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  fvPatchField<vector>::operator=
  (
    valueFraction()*(inletDir_*(inletDir_ & pvf)) + (1 - valueFraction())*pvf
  );
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureDirectedInletOutletVelocityFvPatchVectorField
);

}

