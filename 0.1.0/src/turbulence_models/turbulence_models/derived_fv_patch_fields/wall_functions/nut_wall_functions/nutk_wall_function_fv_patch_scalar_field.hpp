// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::nutkWallFunctionFvPatchScalarField
// Group
//   grpWallFunctions
// Description
//   This boundary condition provides a turbulent kinematic viscosity condition
//   when using wall functions, based on turbulence kinetic energy.
//   - replicates OpenFOAM v1.5 (and earlier) behaviour
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            nutkWallFunction;
//   }
//   \endverbatim
// SeeAlso
//   mousse::nutWallFunctionFvPatchScalarField
// SourceFiles
//   nutk_wall_function_fv_patch_scalar_field.cpp
#ifndef nutk_wall_function_fv_patch_scalar_field_hpp_
#define nutk_wall_function_fv_patch_scalar_field_hpp_
#include "nut_wall_function_fv_patch_scalar_field.hpp"
namespace mousse
{
class nutkWallFunctionFvPatchScalarField
:
  public nutWallFunctionFvPatchScalarField
{
protected:
  // Protected Member Functions
    //- Calculate the turbulence viscosity
    virtual tmp<scalarField> calcNut() const;
public:
  //- Runtime type information
  TypeName("nutkWallFunction");
  // Constructors
    //- Construct from patch and internal field
    nutkWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    nutkWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  nutkWallFunctionFvPatchScalarField
    //  onto a new patch
    nutkWallFunctionFvPatchScalarField
    (
      const nutkWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    nutkWallFunctionFvPatchScalarField
    (
      const nutkWallFunctionFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new nutkWallFunctionFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    nutkWallFunctionFvPatchScalarField
    (
      const nutkWallFunctionFvPatchScalarField&,
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
        new nutkWallFunctionFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    //- Calculate and return the yPlus at the boundary
    virtual tmp<scalarField> yPlus() const;
};
}  // namespace mousse
#endif
