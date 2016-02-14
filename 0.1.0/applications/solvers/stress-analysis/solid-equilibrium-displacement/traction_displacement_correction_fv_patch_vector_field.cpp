// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "traction_displacement_correction_fv_patch_vector_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"
namespace mousse
{
// Constructors 
tractionDisplacementCorrectionFvPatchVectorField::
tractionDisplacementCorrectionFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedGradientFvPatchVectorField{p, iF},
  traction_{p.size(), vector::zero},
  pressure_{p.size(), 0.0}
{
  fvPatchVectorField::operator=(patchInternalField());
  gradient() = vector::zero;
}
tractionDisplacementCorrectionFvPatchVectorField::
tractionDisplacementCorrectionFvPatchVectorField
(
  const tractionDisplacementCorrectionFvPatchVectorField& tdpvf,
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedGradientFvPatchVectorField{tdpvf, p, iF, mapper},
  traction_{tdpvf.traction_, mapper},
  pressure_{tdpvf.pressure_, mapper}
{}
tractionDisplacementCorrectionFvPatchVectorField::
tractionDisplacementCorrectionFvPatchVectorField
(
  const fvPatch& p,
  const DimensionedField<vector, volMesh>& iF,
  const dictionary& dict
)
:
  fixedGradientFvPatchVectorField{p, iF},
  traction_{"traction", dict, p.size()},
  pressure_{"pressure", dict, p.size()}
{
  fvPatchVectorField::operator=(patchInternalField());
  gradient() = vector::zero;
}
tractionDisplacementCorrectionFvPatchVectorField::
tractionDisplacementCorrectionFvPatchVectorField
(
  const tractionDisplacementCorrectionFvPatchVectorField& tdpvf
)
:
  fixedGradientFvPatchVectorField{tdpvf},
  traction_{tdpvf.traction_},
  pressure_{tdpvf.pressure_}
{}
tractionDisplacementCorrectionFvPatchVectorField::
tractionDisplacementCorrectionFvPatchVectorField
(
  const tractionDisplacementCorrectionFvPatchVectorField& tdpvf,
  const DimensionedField<vector, volMesh>& iF
)
:
  fixedGradientFvPatchVectorField{tdpvf, iF},
  traction_{tdpvf.traction_},
  pressure_{tdpvf.pressure_}
{}
// Member Functions 
void tractionDisplacementCorrectionFvPatchVectorField::autoMap
(
  const fvPatchFieldMapper& m
)
{
  fixedGradientFvPatchVectorField::autoMap(m);
  traction_.autoMap(m);
  pressure_.autoMap(m);
}
// Reverse-map the given fvPatchField onto this fvPatchField
void tractionDisplacementCorrectionFvPatchVectorField::rmap
(
  const fvPatchVectorField& ptf,
  const labelList& addr
)
{
  fixedGradientFvPatchVectorField::rmap(ptf, addr);
  const tractionDisplacementCorrectionFvPatchVectorField& dmptf =
    refCast<const tractionDisplacementCorrectionFvPatchVectorField>(ptf);
  traction_.rmap(dmptf.traction_, addr);
  pressure_.rmap(dmptf.pressure_, addr);
}
// Update the coefficients associated with the patch field
void tractionDisplacementCorrectionFvPatchVectorField::updateCoeffs()
{
  if (updated())
  {
    return;
  }
  const dictionary& mechanicalProperties = db().lookupObject<IOdictionary>
  (
    "mechanicalProperties"
  );
  const fvPatchField<scalar>& rho =
    patch().lookupPatchField<volScalarField, scalar>("rho");
  const fvPatchField<scalar>& rhoE =
    patch().lookupPatchField<volScalarField, scalar>("E");
  const fvPatchField<scalar>& nu =
    patch().lookupPatchField<volScalarField, scalar>("nu");
  scalarField E{rhoE/rho};
  scalarField mu{E/(2.0*(1.0 + nu))};
  scalarField lambda{nu*E/((1.0 + nu)*(1.0 - 2.0*nu))};
  Switch planeStress{mechanicalProperties.lookup("planeStress")};
  if (planeStress)
  {
    lambda = nu*E/((1.0 + nu)*(1.0 - nu));
  }
  vectorField n{patch().nf()};
  const fvPatchField<symmTensor>& sigmaD =
    patch().lookupPatchField<volSymmTensorField, symmTensor>("sigmaD");
  const fvPatchField<tensor>& sigmaExp =
    patch().lookupPatchField<volTensorField, tensor>("sigmaExp");
  gradient() =
  (
    (traction_ + pressure_*n)/rho - (n & (sigmaD + sigmaExp))
  )/(2.0*mu + lambda);
  fixedGradientFvPatchVectorField::updateCoeffs();
}
// Write
void tractionDisplacementCorrectionFvPatchVectorField::write(Ostream& os) const
{
  fvPatchVectorField::write(os);
  traction_.writeEntry("traction", os);
  pressure_.writeEntry("pressure", os);
  writeEntry("value", os);
}
MAKE_PATCH_TYPE_FIELD
(
  fvPatchVectorField,
  tractionDisplacementCorrectionFvPatchVectorField
);
}  // namespace mousse