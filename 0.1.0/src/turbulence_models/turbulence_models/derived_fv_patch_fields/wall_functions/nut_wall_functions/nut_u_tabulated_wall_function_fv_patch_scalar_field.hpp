#ifndef TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_TABULATED_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_TURBULENCE_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_NUT_WALL_FUNCTIONS_NUT_U_TABULATED_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutUTabulatedWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions.  As input, the user specifies a look-up table
//   of U+ as a function of near-wall Reynolds number.  The table should be
//   located in the $FOAM_CASE/constant directory.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     uPlusTable   | U+ as a function of Re table name | yes |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutTabulatedWallFunction;
//     uPlusTable      myUPlusTable;
//   }
//   \endverbatim
// Note
//   The tables are not registered since the same table object may be used for
//   more than one patch.
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField
// SourceFiles
//   nut_u_tabulated_wall_function_fv_patch_scalar_field.cpp
#include "nut_wall_function_fv_patch_scalar_field.hpp"
#include "uniform_interpolation_table.hpp"
namespace mousse
{
class nutUTabulatedWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
protected:
  // Protected data
    //- Name of u+ table
    word uPlusTableName_;
    //- U+ table
    uniformInterpolationTable<scalar> uPlusTable_;
  // Protected Member Functions
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
    //- Calculate wall u+ from table
    virtual tmp<scalarField> calcUPlus(const scalarField& Rey) const;
public:
  //- Runtime type information
  TYPE_NAME("nutTabulatedWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutUTabulatedWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutUTabulatedWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutUTabulatedWallFunctionFvPatchScalarField
    //  onto a new patch
    nutUTabulatedWallFunctionFvPatchScalarField
    (
      const nutUTabulatedWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutUTabulatedWallFunctionFvPatchScalarField
    (
      const nutUTabulatedWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new nutUTabulatedWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nutUTabulatedWallFunctionFvPatchScalarField
    (
      const nutUTabulatedWallFunctionFvPatchScalarField&,
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
        new nutUTabulatedWallFunctionFvPatchScalarField(*this, iF)
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
