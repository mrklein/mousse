#ifndef THERMOPHYSICAL_MODELS_BASIC_DERIVED_FV_PATCH_FIELDS_FIXED_ENERGY_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_DERIVED_FV_PATCH_FIELDS_FIXED_ENERGY_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedEnergyFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a fixed condition for internal energy
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            fixedEnergy;
//     value           uniform 100;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   fixed_energy_fv_patch_scalar_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class fixedEnergyFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("fixedEnergy");
  // Constructors
    //- Construct from patch and internal field
    fixedEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given fixedEnergyFvPatchScalarField
    // onto a new patch
    fixedEnergyFvPatchScalarField
    (
      const fixedEnergyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedEnergyFvPatchScalarField
    (
      const fixedEnergyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new fixedEnergyFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedEnergyFvPatchScalarField
    (
      const fixedEnergyFvPatchScalarField&,
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
        new fixedEnergyFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};
}  // namespace mousse
#endif
