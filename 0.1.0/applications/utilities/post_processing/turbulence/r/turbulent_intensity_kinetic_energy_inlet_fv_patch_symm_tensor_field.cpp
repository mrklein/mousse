// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulent_intensity_kinetic_energy_inlet_fv_patch_symm_tensor_field.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "vol_fields.hpp"


// Constructors 
mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
  const fvPatch& p,
  const DimensionedField<symmTensor, volMesh>& iF
)
:
  fixedValueFvPatchSymmTensorField{p, iF}
{}


mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
  const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf,
  const fvPatch& p,
  const DimensionedField<symmTensor, volMesh>& iF,
  const fvPatchFieldMapper& mapper
)
:
  fixedValueFvPatchSymmTensorField{ptf, p, iF, mapper}
{}


mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
  const fvPatch& p,
  const DimensionedField<symmTensor, volMesh>& iF,
  const dictionary& dict
)
:
  fixedValueFvPatchSymmTensorField{p, iF}
{
  fvPatchSymmTensorField::operator=(symmTensorField{"value", dict, p.size()});
}


mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
  const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf
)
:
  fixedValueFvPatchSymmTensorField{ptf}
{}


mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::
turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
(
  const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField& ptf,
  const DimensionedField<symmTensor, volMesh>& iF
)
:
  fixedValueFvPatchSymmTensorField{ptf, iF}
{}


// Member Functions 
void mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField::write
(
  Ostream& os
) const
{
  fvPatchSymmTensorField::write(os);
  writeEntry("value", os);
}


namespace mousse {

MAKE_PATCH_TYPE_FIELD
(
  fvPatchSymmTensorField,
  turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
);

}

