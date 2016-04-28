#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_LOW_RE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_LOW_RE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutLowReWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   for use with low Reynolds number models.  It sets \c nut to zero, and
//   provides an access function to calculate y+.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutLowReWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField

#include "nut_wall_function_fv_patch_scalar_field.hpp"


namespace mousse {

class nutLowReWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
protected:
  // Protected Member Functions
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
public:
  //- Runtime type information
  TYPE_NAME("nutLowReWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutLowReWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutLowReWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutLowReWallFunctionFvPatchScalarField
    //  onto a new patch
    nutLowReWallFunctionFvPatchScalarField
    (
      const nutLowReWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutLowReWallFunctionFvPatchScalarField
    (
      const nutLowReWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new nutLowReWallFunctionFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    nutLowReWallFunctionFvPatchScalarField
    (
      const nutLowReWallFunctionFvPatchScalarField&,
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
          new nutLowReWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    //- Calculate and return the yPlus at the boundary
    virtual tmp<scalarField> yPlus() const;
};

}  // namespace mousse

#endif

