// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::alphatWallFunctionFvPatchScalarField
// Group
//   grpCmpWallFunctions
// Description
//   This boundary condition provides a turbulent thermal diffusivity conditon
//   when using wall functions
//   - replicates OpenFOAM v1.5 (and earlier) behaviour
//   The turbulent thermal diffusivity calculated using:
//     \f[
//       \alpha_t = \frac{\mu_t}{Pr_t}
//     \f]
//   where
//   \vartable
//     \alpha_t| turblence thermal diffusivity
//     \mu_t   | turblence viscosity
//     Pr_t    | turblent Prandtl number
//   \endvartable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     nut          | turbulence viscosity field name | no  | nut
//     Prt          | turbulent Prandtl number | no          | 0.85
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            alphatWallFunction;
//     nut             nut;
//     Prt             0.85;
//     value           uniform 0; // optional value entry
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   alphat_wall_function_fv_patch_scalar_field.cpp
#ifndef compressibleAlphatWallFunctionFvPatchScalarField_H
#define compressibleAlphatWallFunctionFvPatchScalarField_H
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
namespace compressible
{
class alphatWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Turbulent Prandtl number (default = 0.85)
    scalar Prt_;
public:
  //- Runtime type information
  TypeName("compressible::alphatWallFunction");
  // Constructors
    //- Construct from patch and internal field
    alphatWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphatWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  alphatWallFunctionFvPatchScalarField
    //  onto a new patch
    alphatWallFunctionFvPatchScalarField
    (
      const alphatWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphatWallFunctionFvPatchScalarField
    (
      const alphatWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new alphatWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    alphatWallFunctionFvPatchScalarField
    (
      const alphatWallFunctionFvPatchScalarField&,
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
        new alphatWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace compressible
}  // namespace mousse
#endif
