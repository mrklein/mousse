#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUTK_ROUGH_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUTK_ROUGH_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutkRoughWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions for rough walls, based on turbulence kinetic
//   energy.  The condition manipulates the E parameter to account for roughness
//   effects.
//   Parameter ranges
//   - roughness height = sand-grain roughness (0 for smooth walls)
//   - roughness constant = 0.5-1.0
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     Ks           | sand-grain roughness height | yes     |
//     Cs           | roughness constant      | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutkRoughWallFunction;
//     Ks              uniform 0;
//     Cs              uniform 0.5;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutkRoughWallFunctionFvPatchScalarField
// SourceFiles
//   nutk_rough_wall_function_fv_patch_scalar_field.cpp
#include "nutk_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
class nutkRoughWallFunctionFvPatchScalarField
:
  public nutkWallFunctionFvPatchScalarField
{
protected:
  // Protected data
    //- Roughness height
    scalarField Ks_;
    //- Roughness constant
    scalarField Cs_;
  // Protected Member Functions
    //- Compute the roughness function
    virtual scalar fnRough(const scalar KsPlus, const scalar Cs) const;
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
public:
  //- Runtime type information
  TYPE_NAME("nutkRoughWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutkRoughWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutkRoughWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutkRoughWallFunctionFvPatchScalarField
    //  onto a new patch
    nutkRoughWallFunctionFvPatchScalarField
    (
      const nutkRoughWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutkRoughWallFunctionFvPatchScalarField
    (
      const nutkRoughWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new nutkRoughWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nutkRoughWallFunctionFvPatchScalarField
    (
      const nutkRoughWallFunctionFvPatchScalarField&,
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
        new nutkRoughWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Acces functions
      // Return Ks
      scalarField& Ks()
      {
        return Ks_;
      }
      // Return Cs
      scalarField& Cs()
      {
        return Cs_;
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap(const fvPatchFieldMapper&);
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
