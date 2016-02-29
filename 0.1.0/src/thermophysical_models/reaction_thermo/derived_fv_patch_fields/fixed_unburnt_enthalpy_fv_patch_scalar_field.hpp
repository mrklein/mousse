#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_FIXED_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_FIXED_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedUnburntEnthalpyFvPatchScalarField
// Description
//   Fixed boundary condition for unburnt
// SourceFiles
//   fixed_unburnt_enthalpy_fv_patch_scalar_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class fixedUnburntEnthalpyFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("fixedUnburntEnthalpy");
  // Constructors
    //- Construct from patch and internal field
    fixedUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedUnburntEnthalpyFvPatchScalarField
    // onto a new patch
    fixedUnburntEnthalpyFvPatchScalarField
    (
      const fixedUnburntEnthalpyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedUnburntEnthalpyFvPatchScalarField
    (
      const fixedUnburntEnthalpyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new fixedUnburntEnthalpyFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedUnburntEnthalpyFvPatchScalarField
    (
      const fixedUnburntEnthalpyFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      (
        new fixedUnburntEnthalpyFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};
}  // namespace mousse
#endif
