#ifndef TRANSPORT_MODELS_TWO_PHASE_PROPERTIES_DYNAMIC_ALPHA_CONTACT_ANGLE_FV_PATCH_SCALAR_FIELD_HPP_
#define TRANSPORT_MODELS_TWO_PHASE_PROPERTIES_DYNAMIC_ALPHA_CONTACT_ANGLE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicAlphaContactAngleFvPatchScalarField
// Description
//   A dynamic alphaContactAngle scalar boundary condition
//   (alphaContactAngleFvPatchScalarField)

#include "alpha_contact_angle_fv_patch_scalar_field.hpp"


namespace mousse {

class dynamicAlphaContactAngleFvPatchScalarField
:
  public alphaContactAngleFvPatchScalarField
{
  // Private data
    //- Equilibrium contact angle
    scalar theta0_;
    //- Dynamic contact angle velocity scale
    scalar uTheta_;
    //- Limiting advancing contact angle
    scalar thetaA_;
    //- Limiting receeding contact angle
    scalar thetaR_;
public:
  //- Runtime type information
  TYPE_NAME("dynamicAlphaContactAngle");
  // Constructors
    //- Construct from patch and internal field
    dynamicAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    dynamicAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  dynamicAlphaContactAngleFvPatchScalarField
    //  onto a new patch
    dynamicAlphaContactAngleFvPatchScalarField
    (
      const dynamicAlphaContactAngleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    dynamicAlphaContactAngleFvPatchScalarField
    (
      const dynamicAlphaContactAngleFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new dynamicAlphaContactAngleFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    dynamicAlphaContactAngleFvPatchScalarField
    (
      const dynamicAlphaContactAngleFvPatchScalarField&,
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
          new dynamicAlphaContactAngleFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    //- Evaluate and return dynamic contact-angle
    virtual tmp<scalarField> theta
    (
      const fvPatchVectorField& Up,
      const fvsPatchVectorField& nHat
    ) const;
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

