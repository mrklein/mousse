#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED__FREESTREAM_PRESSURE_FV_PATCH_SCALAR_FIELDS_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED__FREESTREAM_PRESSURE_FV_PATCH_SCALAR_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::freestreamPressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a free-stream condition for pressure.
//   It is a zero-gradient condition that constrains the flux across the patch
//   based on the free-stream velocity.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velocity field name     | no          | U
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | none
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            freestreamPressure;
//   }
//   \endverbatim
// Note
//   This condition is designed to operate with a freestream velocity condition
// SeeAlso
//   mousse::zeroGradientFvPatchField
//   mousse::freestreamFvPatchField
// SourceFiles
//   freestream_pressure_fv_patch_scalar_field.cpp

#include "fv_patch_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"


namespace mousse {

class freestreamPressureFvPatchScalarField
:
  public zeroGradientFvPatchScalarField
{
  // Private data
    //- Name of the velocity field
    word UName_;
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of the density field used to normalise the mass flux
    //  if neccessary
    word rhoName_;
public:
  //- Runtime type information
  TYPE_NAME("freestreamPressure");
  // Constructors
    //- Construct from patch and internal field
    freestreamPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    freestreamPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given freestreamPressureFvPatchScalarField onto
    //  a new patch
    freestreamPressureFvPatchScalarField
    (
      const freestreamPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    freestreamPressureFvPatchScalarField
    (
      const freestreamPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new freestreamPressureFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    freestreamPressureFvPatchScalarField
    (
      const freestreamPressureFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      {
        new freestreamPressureFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
