#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_WALL_FUNCTIONS_ALPHAT_PHASE_CHANGE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_WALL_FUNCTIONS_ALPHAT_WALL_FUNCTIONS_ALPHAT_PHASE_CHANGE_WALL_FUNCTION_FV_PATCH_SCALAR_FIELD_HPP_

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

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {
namespace compressible {

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
  TYPE_NAME("compressible::alphatPhaseChangeWallFunction");
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

