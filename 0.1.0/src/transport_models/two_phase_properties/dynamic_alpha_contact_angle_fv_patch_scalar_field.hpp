// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::dynamicAlphaContactAngleFvPatchScalarField
// Description
//   A dynamic alphaContactAngle scalar boundary condition
//   (alphaContactAngleFvPatchScalarField)
// SourceFiles
//   dynamic_alpha_contact_angle_fv_patch_scalar_field.cpp
#ifndef dynamic_alpha_contact_angle_fv_patch_scalar_field_hpp_
#define dynamic_alpha_contact_angle_fv_patch_scalar_field_hpp_
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
namespace mousse
{
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
  TypeName("dynamicAlphaContactAngle");
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
      return tmp<fvPatchScalarField>
      (
        new dynamicAlphaContactAngleFvPatchScalarField(*this)
      );
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
      return tmp<fvPatchScalarField>
      (
        new dynamicAlphaContactAngleFvPatchScalarField(*this, iF)
      );
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
