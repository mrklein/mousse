// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::incompressible::alphatJayatillekeWallFunctionFvPatchScalarField
// Group
//   grpIcoWallFunctions
// Description
//   This boundary condition provides a kinematic turbulent thermal conductivity
//   for using wall functions, using the Jayatilleke 'P' function.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Prt          | turbulent Prandtl number | no         | 0.85
//     Cmu          | model coefficient       | no          | 0.09
//     kappa        | Von Karman constant     | no          | 0.41
//     E            | model coefficient       | no          | 9.8
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            alphatJayatillekeWallFunction;
//   }
//   \endverbatim
// Note
//   The units of kinematic turbulent thermal conductivity are [m2/s]
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   alphat_jayatilleke_wall_function_fv_patch_scalar_field.cpp
#ifndef alphat_jayatilleke_wall_function_fv_patch_scalar_field_hpp_
#define alphat_jayatilleke_wall_function_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
namespace incompressible
{
class alphatJayatillekeWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
protected:
  // Protected data
    //- Turbulent Prandtl number
    scalar Prt_;
    //- Cmu coefficient
    scalar Cmu_;
    //- Von Karman constant
    scalar kappa_;
    //- E coefficient
    scalar E_;
    // Solution parameters
      static scalar tolerance_;
      static label maxIters_;
  // Protected member functions
    //- Check the type of the patch
    virtual void checkType();
    //- `P' function
    scalar Psmooth(const scalar Prat) const;
    //- Calculate y+ at the edge of the thermal laminar sublayer
    scalar yPlusTherm
    (
      const scalar P,
      const scalar Prat
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("alphatJayatillekeWallFunction");
  // Constructors
    //- Construct from patch and internal field
    alphatJayatillekeWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphatJayatillekeWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  alphatJayatillekeWallFunctionFvPatchScalarField
    //  onto a new patch
    alphatJayatillekeWallFunctionFvPatchScalarField
    (
      const alphatJayatillekeWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphatJayatillekeWallFunctionFvPatchScalarField
    (
      const alphatJayatillekeWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new alphatJayatillekeWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    alphatJayatillekeWallFunctionFvPatchScalarField
    (
      const alphatJayatillekeWallFunctionFvPatchScalarField&,
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
        new alphatJayatillekeWallFunctionFvPatchScalarField(*this, iF)
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
}  // namespace incompressible
}  // namespace mousse
#endif
