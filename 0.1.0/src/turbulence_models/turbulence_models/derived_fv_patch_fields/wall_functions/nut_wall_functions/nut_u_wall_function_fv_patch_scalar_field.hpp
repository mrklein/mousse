#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutUWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions, based on velocity.
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutUWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField
// SourceFiles
//   nut_u_wall_function_fv_patch_scalar_field.cpp
#include "nut_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
class nutUWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
protected:
  // Protected Member Functions
    //- Calculate yPLus
    virtual tmp<scalarField> calcYPlus(const scalarField& magUp) const;
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
public:
  //- Runtime type information
  TYPE_NAME("nutUWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutUWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutUWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutUWallFunctionFvPatchScalarField
    //  onto a new patch
    nutUWallFunctionFvPatchScalarField
    (
      const nutUWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutUWallFunctionFvPatchScalarField
    (
      const nutUWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new nutUWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nutUWallFunctionFvPatchScalarField
    (
      const nutUWallFunctionFvPatchScalarField&,
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
        new nutUWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Calculate and return the yPlus at the boundary
      virtual tmp<scalarField> yPlus() const;
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};
}  // namespace mousse
#endif
