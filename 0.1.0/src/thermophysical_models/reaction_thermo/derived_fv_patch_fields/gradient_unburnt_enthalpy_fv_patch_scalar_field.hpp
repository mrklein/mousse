#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_GRADIENT_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_GRADIENT_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::gradientUnburntEnthalpyFvPatchScalarField
// Description
//   gradient boundary condition for unburnt

#include "fixed_gradient_fv_patch_fields.hpp"


namespace mousse {

class gradientUnburntEnthalpyFvPatchScalarField
:
  public fixedGradientFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("gradientUnburntEnthalpy");
  // Constructors
    //- Construct from patch and internal field
    gradientUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    gradientUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given gradientUnburntEnthalpyFvPatchScalarField
    // onto a new patch
    gradientUnburntEnthalpyFvPatchScalarField
    (
      const gradientUnburntEnthalpyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    gradientUnburntEnthalpyFvPatchScalarField
    (
      const gradientUnburntEnthalpyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new gradientUnburntEnthalpyFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    gradientUnburntEnthalpyFvPatchScalarField
    (
      const gradientUnburntEnthalpyFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new gradientUnburntEnthalpyFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};

}  // namespace mousse

#endif

