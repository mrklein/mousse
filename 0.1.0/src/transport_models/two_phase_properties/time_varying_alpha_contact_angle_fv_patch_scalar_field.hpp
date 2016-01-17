// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::timeVaryingAlphaContactAngleFvPatchScalarField
// Description
//   A time-varying alphaContactAngle scalar boundary condition
//   (alphaContactAngleFvPatchScalarField)
// SourceFiles
//   time_varying_alpha_contact_angle_fv_patch_scalar_field.cpp
#ifndef time_varying_alpha_contact_angle_fv_patch_scalar_field_hpp_
#define time_varying_alpha_contact_angle_fv_patch_scalar_field_hpp_
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
namespace mousse
{
class timeVaryingAlphaContactAngleFvPatchScalarField
:
  public alphaContactAngleFvPatchScalarField
{
  // Private data
    // Equilibrium contact angle control parameters
    scalar t0_;
    scalar thetaT0_;
    scalar te_;
    scalar thetaTe_;
public:
  //- Runtime type information
  TYPE_NAME("timeVaryingAlphaContactAngle");
  // Constructors
    //- Construct from patch and internal field
    timeVaryingAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    timeVaryingAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  timeVaryingAlphaContactAngleFvPatchScalarField onto a new patch
    timeVaryingAlphaContactAngleFvPatchScalarField
    (
      const timeVaryingAlphaContactAngleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new timeVaryingAlphaContactAngleFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    timeVaryingAlphaContactAngleFvPatchScalarField
    (
      const timeVaryingAlphaContactAngleFvPatchScalarField&,
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
        new timeVaryingAlphaContactAngleFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    //- Evaluate and return the time-varying equilibrium contact-angle
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
