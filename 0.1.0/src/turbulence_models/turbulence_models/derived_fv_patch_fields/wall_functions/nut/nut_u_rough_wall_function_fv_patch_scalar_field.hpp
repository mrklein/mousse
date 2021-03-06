#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_ROUGH_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_ROUGH_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutURoughWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions for rough walls, based on velocity.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     roughnessHeight | roughness height     | yes         |
//     roughnessConstant | roughness constanr | yes         |
//     roughnessFactor | scaling factor       | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutURoughWallFunction;
//     roughnessHeight 1e-5;
//     roughnessConstant 0.5;
//     roughnessFactor 1;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField

#include "nut_wall_function_fv_patch_scalar_field.hpp"


namespace mousse {

class nutURoughWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
  // Private data
    // Roughness model parameters
      //- Height
      scalar roughnessHeight_;
      //- Constant
      scalar roughnessConstant_;
      //- Scale factor
      scalar roughnessFactor_;
  // Protected Member Functions
    //- Calculate yPLus
    virtual tmp<scalarField> calcYPlus(const scalarField& magUp) const;
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
public:
  //- Runtime type information
  TYPE_NAME("nutURoughWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutURoughWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutURoughWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutURoughWallFunctionFvPatchScalarField
    //  onto a new patch
    nutURoughWallFunctionFvPatchScalarField
    (
      const nutURoughWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutURoughWallFunctionFvPatchScalarField
    (
      const nutURoughWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new nutURoughWallFunctionFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    nutURoughWallFunctionFvPatchScalarField
    (
      const nutURoughWallFunctionFvPatchScalarField&,
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
          new nutURoughWallFunctionFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Access
      //- Return the roughness height
      scalar roughnessHeight() const { return roughnessHeight_; }
      //- Return reference to the roughness height to allow adjustment
      scalar& roughnessHeight() { return roughnessHeight_; }
      //- Return the roughness constant scale
      scalar roughnessConstant() const { return roughnessConstant_; }
      //- Return reference to the roughness constant to allow adjustment
      scalar& roughnessConstant() { return roughnessConstant_; }
      //- Return the roughness scale factor
      scalar roughnessFactor() const { return roughnessFactor_; }
      //- Return reference to the roughness scale factor to allow
      //  adjustment
      scalar& roughnessFactor() { return roughnessFactor_; }
  // I-O
    // Evaluation functions
      //- Calculate and return the yPlus at the boundary
      virtual tmp<scalarField> yPlus() const;
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};

}  // namespace mousse

#endif

