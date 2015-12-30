// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::alphatPhaseChangeWallFunctionFvPatchScalarField
// Group
//   grpCmpWallFunctions
// Description
//   Abstract base-class for all alphatWallFunctions supporting phase-change.
// SeeAlso
//   mousse::alphatWallFunction
// SourceFiles
//   alphat_phase_change_wall_function_fv_patch_scalar_field.cpp
#ifndef compressibleAlphatPhaseChangeWallFunctionFvPatchScalarField_H
#define compressibleAlphatPhaseChangeWallFunctionFvPatchScalarField_H
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
namespace compressible
{
class alphatPhaseChangeWallFunctionFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
protected:
  // Protected data
    //- Rate of phase-change
    scalarField dmdt_;
public:
  //- Runtime type information
  TypeName("compressible::alphatPhaseChangeWallFunction");
  // Constructors
    //- Construct from patch and internal field
    alphatPhaseChangeWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    alphatPhaseChangeWallFunctionFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  alphatPhaseChangeWallFunctionFvPatchScalarField
    //  onto a new patch
    alphatPhaseChangeWallFunctionFvPatchScalarField
    (
      const alphatPhaseChangeWallFunctionFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    alphatPhaseChangeWallFunctionFvPatchScalarField
    (
      const alphatPhaseChangeWallFunctionFvPatchScalarField&
    );
    //- Construct as copy setting internal field reference
    alphatPhaseChangeWallFunctionFvPatchScalarField
    (
      const alphatPhaseChangeWallFunctionFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
  // Member functions
    //- Return the rate of phase-change
    const scalarField& dmdt() const
    {
      return dmdt_;
    }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs() = 0;
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace compressible
}  // namespace mousse
#endif
