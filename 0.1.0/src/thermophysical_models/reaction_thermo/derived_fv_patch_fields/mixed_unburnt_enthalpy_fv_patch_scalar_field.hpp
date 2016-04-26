#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_MIXED_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_DERIVED_FV_PATCH_FIELDS_MIXED_UNBURNT_ENTHALPY_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixedUnburntEnthalpyFvPatchScalarField
// Description
//   Mixed boundary condition for unburnt

#include "mixed_fv_patch_fields.hpp"


namespace mousse {

class mixedUnburntEnthalpyFvPatchScalarField
:
  public mixedFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("mixedUnburntEnthalpy");
  // Constructors
    //- Construct from patch and internal field
    mixedUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mixedUnburntEnthalpyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given mixedUnburntEnthalpyFvPatchScalarField
    // onto a new patch
    mixedUnburntEnthalpyFvPatchScalarField
    (
      const mixedUnburntEnthalpyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mixedUnburntEnthalpyFvPatchScalarField
    (
      const mixedUnburntEnthalpyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new mixedUnburntEnthalpyFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    mixedUnburntEnthalpyFvPatchScalarField
    (
      const mixedUnburntEnthalpyFvPatchScalarField&,
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
          new mixedUnburntEnthalpyFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};

}  // namespace mousse

#endif

