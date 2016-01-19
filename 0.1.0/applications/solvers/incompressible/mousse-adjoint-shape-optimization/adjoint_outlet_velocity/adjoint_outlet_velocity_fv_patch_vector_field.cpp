// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "adjoint_outlet_velocity_fv_patch_vector_field.hpp"
#include "vol_fields.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "surface_fields.hpp"
#include "fv_patch_field_mapper.hpp"

// Constructors 
mousse::adjointOutletVelocityFvPatchVectorField::
adjointOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{p, iF}
{}
mousse::adjointOutletVelocityFvPatchVectorField::
adjointOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchVectorField{p, iF}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
}
mousse::adjointOutletVelocityFvPatchVectorField::
adjointOutletVelocityFvPatchVectorField
(
  const adjointOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchVectorField{ptf, p, iF, mapper}
{}
mousse::adjointOutletVelocityFvPatchVectorField::
adjointOutletVelocityFvPatchVectorField
(
  const adjointOutletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedValueFvPatchVectorField{pivpvf, iF}
{}
// Member Functions 
// Update the coefficients associated with the patch field
void mousse::adjointOutletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const fvsPatchField<scalar>& phiap =
    patch().lookupPatchField<surfaceScalarField, scalar>("phia");
  const fvPatchField<vector>& Up =
    patch().lookupPatchField<volVectorField, vector>("U");
  scalarField Un{mag(patch().nf() & Up)};
  vectorField UtHat{(Up - patch().nf()*Un)/(Un + SMALL)};
  vectorField Uan{patch().nf()*(patch().nf() & patchInternalField())};
  vectorField::operator=(phiap*patch().Sf()/sqr(patch().magSf()) + UtHat);
  //vectorField::operator=(Uan + UtHat);
  fixedValueFvPatchVectorField::updateCoeffs();
}
void mousse::adjointOutletVelocityFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  writeEntry("value", os);
}
namespace mousse
{
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  adjointOutletVelocityFvPatchVectorField
);
}
