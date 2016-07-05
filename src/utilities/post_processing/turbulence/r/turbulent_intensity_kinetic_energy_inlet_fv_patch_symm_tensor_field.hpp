#ifndef UTILITIES_POST_PROCESSING_TURBULENCE_R_TURBULENT_INTENSITY_KINETIC_ENERGY_INLET_FV_PATCH_SYMM_TENSOR_FIELD_HPP_
#define UTILITIES_POST_PROCESSING_TURBULENCE_R_TURBULENT_INTENSITY_KINETIC_ENERGY_INLET_FV_PATCH_SYMM_TENSOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
// Description
//   Dummy version of turbulentIntensityKineticEnergyInlet patch type for
//   symmetric tensors provided for compatibility with R utility.
//   Value is given by a fixed-value condition.

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
:
  public fixedValueFvPatchSymmTensorField
{
public:
  //- Runtime type information
  TYPE_NAME("turbulentIntensityKineticEnergyInlet");
  // Constructors
    //- Construct from patch and internal field
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    (
      const fvPatch&,
      const DimensionedField<symmTensor, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    (
      const fvPatch&,
      const DimensionedField<symmTensor, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    //  onto a new patch
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    (
      const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&,
      const fvPatch&,
      const DimensionedField<symmTensor, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    (
      const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchSymmTensorField> clone() const
    {
      return
        tmp<fvPatchSymmTensorField>
        {
          new turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
          {
            *this
          }
        };
    }
    //- Construct as copy setting internal field reference
    turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
    (
      const turbulentIntensityKineticEnergyInletFvPatchSymmTensorField&,
      const DimensionedField<symmTensor, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchSymmTensorField> clone
    (
      const DimensionedField<symmTensor, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchSymmTensorField>
        {
          new turbulentIntensityKineticEnergyInletFvPatchSymmTensorField
          {
            *this,
              iF
          }
        };
    }
  // Member functions
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

