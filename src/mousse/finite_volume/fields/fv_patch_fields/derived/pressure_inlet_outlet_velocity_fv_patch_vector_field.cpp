// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pressure_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_patch_field_mapper.hpp"
#include "transform_field.hpp"
#include "symm_transform_field.hpp"


// Constructors 

mousse::pressureInletOutletVelocityFvPatchVectorField::
pressureInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  directionMixedFvPatchVectorField{p, iF},
  phiName_{"phi"}
{
  refValue() = vector::zero;
  refGrad() = vector::zero;
  valueFraction() = symmTensor::zero;
}


mousse::pressureInletOutletVelocityFvPatchVectorField::
pressureInletOutletVelocityFvPatchVectorField
(
  const pressureInletOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  directionMixedFvPatchVectorField{ptf, p, iF, mapper},
  phiName_{ptf.phiName_}
{
  if (ptf.tangentialVelocity_.size()) {
    tangentialVelocity_ = mapper(ptf.tangentialVelocity_);
  }
}


mousse::pressureInletOutletVelocityFvPatchVectorField::
pressureInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  directionMixedFvPatchVectorField{p, iF},
  phiName_{dict.lookupOrDefault<word>("phi", "phi")}
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
  if (dict.found("tangentialVelocity")) {
    setTangentialVelocity
    (
      vectorField{"tangentialVelocity", dict, p.size()}
    );
  } else {
    refValue() = vector::zero;
  }
  refGrad() = vector::zero;
  valueFraction() = symmTensor::zero;
}


mousse::pressureInletOutletVelocityFvPatchVectorField::
pressureInletOutletVelocityFvPatchVectorField
(
  const pressureInletOutletVelocityFvPatchVectorField& pivpvf
)
:
  directionMixedFvPatchVectorField{pivpvf},
  phiName_{pivpvf.phiName_},
  tangentialVelocity_{pivpvf.tangentialVelocity_}
{}


mousse::pressureInletOutletVelocityFvPatchVectorField::
pressureInletOutletVelocityFvPatchVectorField
(
  const pressureInletOutletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  directionMixedFvPatchVectorField{pivpvf, iF},
  phiName_{pivpvf.phiName_},
  tangentialVelocity_{pivpvf.tangentialVelocity_}
{}


// Member Functions 
void mousse::pressureInletOutletVelocityFvPatchVectorField::
setTangentialVelocity(const vectorField& tangentialVelocity)
{
  tangentialVelocity_ = tangentialVelocity;
  const vectorField n{patch().nf()};
  refValue() = tangentialVelocity_ - n*(n & tangentialVelocity_);
}


void mousse::pressureInletOutletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  directionMixedFvPatchVectorField::autoMap(m);
  if (tangentialVelocity_.size()) {
    tangentialVelocity_.autoMap(m);
  }
}


void mousse::pressureInletOutletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  directionMixedFvPatchVectorField::rmap(ptf, addr);
  if (tangentialVelocity_.size()) {
    const pressureInletOutletVelocityFvPatchVectorField& tiptf =
      refCast<const pressureInletOutletVelocityFvPatchVectorField>(ptf);
    tangentialVelocity_.rmap(tiptf.tangentialVelocity_, addr);
  }
}


void mousse::pressureInletOutletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated()) {
    return;
  }
  const fvsPatchField<scalar>& phip =
    patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
  valueFraction() = neg(phip)*(I - sqr(patch().nf()));
  directionMixedFvPatchVectorField::updateCoeffs();
  directionMixedFvPatchVectorField::evaluate();
}


void mousse::pressureInletOutletVelocityFvPatchVectorField::write
(
  Ostream& os
)
const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  if (tangentialVelocity_.size()) {
    tangentialVelocity_.writeEntry("tangentialVelocity", os);
  }
  writeEntry("value", os);
}


// Member Operators 
void mousse::pressureInletOutletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  tmp<vectorField> normalValue = transform(valueFraction(), refValue());
  tmp<vectorField> transformGradValue = transform(I - valueFraction(), pvf);
  fvPatchField<vector>::operator=(normalValue + transformGradValue);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  pressureInletOutletVelocityFvPatchVectorField
);

}

