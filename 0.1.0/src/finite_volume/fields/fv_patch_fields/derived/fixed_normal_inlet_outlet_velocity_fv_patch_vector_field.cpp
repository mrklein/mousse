// mousse: CFD toolbox
// Copyright (C) 2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_normal_inlet_outlet_velocity_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "transform_field.hpp"
#include "symm_transform_field.hpp"

// Constructors 
mousse::fixedNormalInletOutletVelocityFvPatchVectorField::
fixedNormalInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  directionMixedFvPatchVectorField(p, iF),
  phiName_("phi"),
  fixTangentialInflow_(true),
  normalVelocity_
  (
    fvPatchVectorField::New("fixedValue", p, iF)
  )
{
  refValue() = vector::zero;
  refGrad() = vector::zero;
  valueFraction() = symmTensor::zero;
}
mousse::fixedNormalInletOutletVelocityFvPatchVectorField::
fixedNormalInletOutletVelocityFvPatchVectorField
(
  const fixedNormalInletOutletVelocityFvPatchVectorField& ptf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  directionMixedFvPatchVectorField(ptf, p, iF, mapper),
  phiName_(ptf.phiName_),
  fixTangentialInflow_(ptf.fixTangentialInflow_),
  normalVelocity_
  (
    fvPatchVectorField::New(ptf.normalVelocity(), p, iF, mapper)
  )
{}
mousse::fixedNormalInletOutletVelocityFvPatchVectorField::
fixedNormalInletOutletVelocityFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  directionMixedFvPatchVectorField(p, iF),
  phiName_(dict.lookupOrDefault<word>("phi", "phi")),
  fixTangentialInflow_(dict.lookup("fixTangentialInflow")),
  normalVelocity_
  (
    fvPatchVectorField::New(p, iF, dict.subDict("normalVelocity"))
  )
{
  fvPatchVectorField::operator=(vectorField("value", dict, p.size()));
  refValue() = normalVelocity();
  refGrad() = vector::zero;
  valueFraction() = symmTensor::zero;
}
mousse::fixedNormalInletOutletVelocityFvPatchVectorField::
fixedNormalInletOutletVelocityFvPatchVectorField
(
  const fixedNormalInletOutletVelocityFvPatchVectorField& pivpvf
)
:
  directionMixedFvPatchVectorField(pivpvf),
  phiName_(pivpvf.phiName_),
  fixTangentialInflow_(pivpvf.fixTangentialInflow_),
  normalVelocity_(pivpvf.normalVelocity().clone())
{}
mousse::fixedNormalInletOutletVelocityFvPatchVectorField::
fixedNormalInletOutletVelocityFvPatchVectorField
(
  const fixedNormalInletOutletVelocityFvPatchVectorField& pivpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  directionMixedFvPatchVectorField(pivpvf, iF),
  phiName_(pivpvf.phiName_),
  fixTangentialInflow_(pivpvf.fixTangentialInflow_),
  normalVelocity_(pivpvf.normalVelocity().clone())
{}
// Member Functions 
void mousse::fixedNormalInletOutletVelocityFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  directionMixedFvPatchVectorField::autoMap(m);
  normalVelocity_->autoMap(m);
}
void mousse::fixedNormalInletOutletVelocityFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  directionMixedFvPatchVectorField::rmap(ptf, addr);
  const fixedNormalInletOutletVelocityFvPatchVectorField& fniovptf =
    refCast<const fixedNormalInletOutletVelocityFvPatchVectorField>(ptf);
  normalVelocity_->rmap(fniovptf.normalVelocity(), addr);
}
void mousse::fixedNormalInletOutletVelocityFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  normalVelocity_->evaluate();
  refValue() = normalVelocity();
  valueFraction() = sqr(patch().nf());
  if (fixTangentialInflow_)
  {
    const fvsPatchField<scalar>& phip =
      patch().lookupPatchField<surfaceScalarField, scalar>(phiName_);
    valueFraction() += neg(phip)*(I - valueFraction());
  }
  directionMixedFvPatchVectorField::updateCoeffs();
  directionMixedFvPatchVectorField::evaluate();
}
void mousse::fixedNormalInletOutletVelocityFvPatchVectorField::write
(
  Ostream& os
)
const
{
  fvPatchVectorField::write(os);
  writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
  os.writeKeyword("fixTangentialInflow")
    << fixTangentialInflow_ << token::END_STATEMENT << nl;
  os.writeKeyword("normalVelocity")
    << nl << indent << token::BEGIN_BLOCK << nl << incrIndent;
  normalVelocity_->write(os);
  os << decrIndent << indent << token::END_BLOCK << endl;
  writeEntry("value", os);
}
// Member Operators 
void mousse::fixedNormalInletOutletVelocityFvPatchVectorField::operator=
(
  const fvPatchField<vector>& pvf
)
{
  tmp<vectorField> normalValue = transform(valueFraction(), refValue());
  tmp<vectorField> transformGradValue = transform(I - valueFraction(), pvf);
  fvPatchField<vector>::operator=(normalValue + transformGradValue);
}
namespace mousse
{

MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  fixedNormalInletOutletVelocityFvPatchVectorField
);

}
