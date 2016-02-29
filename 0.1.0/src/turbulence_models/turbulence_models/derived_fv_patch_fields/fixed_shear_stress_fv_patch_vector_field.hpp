#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_FIXED_SHEAR_STRESS_FV_PATCH_VECTOR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_FIXED_SHEAR_STRESS_FV_PATCH_VECTOR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedShearStressFvPatchVectorField
// Group
//   grpTurbulenceBoundaryConditions
// Description
//   Set a constant shear stress as tau0 = -nuEff dU/dn.
// SourceFiles
//   fixed_shear_stress_fv_patch_vector_field.cpp
#include "fv_patch_fields.hpp"
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class fixedShearStressFvPatchVectorField
:
  public fixedValueFvPatchVectorField
{
  // Private data
    //- Constant shear stress
    const vector tau0_;
public:
  //- Runtime type information
  TYPE_NAME("fixedShearStress");
  // Constructors
    //- Construct from patch and internal field
    fixedShearStressFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedShearStressFvPatchVectorField
    (
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    fixedShearStressFvPatchVectorField
    (
      const fixedShearStressFvPatchVectorField&,
      const fvPatch&,
      const DimensionedField<vector, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedShearStressFvPatchVectorField
    (
      const fixedShearStressFvPatchVectorField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchVectorField> clone() const
    {
      return tmp<fvPatchVectorField>
      (
        new fixedShearStressFvPatchVectorField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedShearStressFvPatchVectorField
    (
      const fixedShearStressFvPatchVectorField&,
      const DimensionedField<vector, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchVectorField> clone
    (
      const DimensionedField<vector, volMesh>& iF
    ) const
    {
      return tmp<fvPatchVectorField>
      (
        new fixedShearStressFvPatchVectorField(*this, iF)
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
