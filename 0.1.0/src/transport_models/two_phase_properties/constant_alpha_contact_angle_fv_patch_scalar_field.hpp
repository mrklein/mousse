// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantAlphaContactAngleFvPatchScalarField
// Description
//   A constant alphaContactAngle scalar boundary condition
//   (alphaContactAngleFvPatchScalarField)
// SourceFiles
//   constant_alpha_contact_angle_fv_patch_scalar_field.cpp
#ifndef constant_alpha_contact_angle_fv_patch_scalar_field_hpp_
#define constant_alpha_contact_angle_fv_patch_scalar_field_hpp_
#include "alpha_contact_angle_fv_patch_scalar_field.hpp"
namespace mousse
{
class constantAlphaContactAngleFvPatchScalarField
:
  public alphaContactAngleFvPatchScalarField
{
  // Private data
    //- Equilibrium contact angle
    scalar theta0_;
public:
  //- Runtime type information
  TypeName("constantAlphaContactAngle");
  // Constructors
    //- Construct from patch and internal field
    constantAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    constantAlphaContactAngleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  constantAlphaContactAngleFvPatchScalarField
    //  onto a new patch
    constantAlphaContactAngleFvPatchScalarField
    (
      const constantAlphaContactAngleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    constantAlphaContactAngleFvPatchScalarField
    (
      const constantAlphaContactAngleFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new constantAlphaContactAngleFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    constantAlphaContactAngleFvPatchScalarField
    (
      const constantAlphaContactAngleFvPatchScalarField&,
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
        new constantAlphaContactAngleFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    //- Return the equilibrium contact-angle
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
