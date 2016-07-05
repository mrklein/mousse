#ifndef TRANSPORT_MODELS_TWO_PHASE_PROPERTIES_ALPHA_FIXED_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_
#define TRANSPORT_MODELS_TWO_PHASE_PROPERTIES_ALPHA_FIXED_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::alphaFixedPressureFvPatchScalarField
// Description
//   A fixed-pressure alphaContactAngle boundary

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class alphaFixedPressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Fixed pressure
    scalarField p_;
public:
  //- Runtime type information
  TYPE_NAME("alphaFixedPressure");
  // Constructors
    //- Construct from patch and internal field
    alphaFixedPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphaFixedPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given alphaFixedPressureFvPatchScalarField
    //  onto a new patch
    alphaFixedPressureFvPatchScalarField
    (
      const alphaFixedPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphaFixedPressureFvPatchScalarField
    (
      const alphaFixedPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new alphaFixedPressureFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    alphaFixedPressureFvPatchScalarField
    (
      const alphaFixedPressureFvPatchScalarField&,
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
          new alphaFixedPressureFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Access
      //- Return the alphaFixed pressure
      const scalarField& p() const
      {
        return p_;
      }
      //- Return reference to the alphaFixed pressure to allow adjustment
      scalarField& p()
      {
        return p_;
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

