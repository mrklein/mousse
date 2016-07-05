#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_WALL_FUNCTIONS_ALPHAT_JAYATILLEKE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_WALL_FUNCTIONS_ALPHAT_JAYATILLEKE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::alphatJayatillekeWallFunctionFvPatchScalarField
// Group
//   grpCmpWallFunctions
// Description
//   This boundary condition provides a thermal wall function for turbulent
//   thermal diffusivity (usually\c alphat) based on the Jayatilleke model.
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
//     Prt             0.85;
//     kappa           0.41;
//     E               9.8;
//     value           uniform 0; // optional value entry
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {
namespace compressible {

class alphatJayatillekeWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Turbulent Prandtl number
    scalar Prt_;
    //- Cmu coefficient
    scalar Cmu_;
    //- Von Karman constant
    scalar kappa_;
    //- E coefficient
    scalar E_;
    // Solution parameters
      static scalar maxExp_;
      static scalar tolerance_;
      static label maxIters_;
  // Private Member Functions
    //- Check the type of the patch
    void checkType();
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
  TYPE_NAME("compressible::alphatJayatillekeWallFunction");
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
    //- Construct by mapping given an
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
      return
        tmp<fvPatchScalarField>
        {
          new alphatJayatillekeWallFunctionFvPatchScalarField{*this}
        };
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
      return
        tmp<fvPatchScalarField>
        {
          new alphatJayatillekeWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      void write(Ostream&) const;
};

}  // namespace compressible
}  // namespace mousse

#endif

