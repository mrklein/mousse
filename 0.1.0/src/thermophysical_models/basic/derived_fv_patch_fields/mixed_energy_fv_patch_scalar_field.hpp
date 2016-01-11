// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::mixedEnergyFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a mixed condition for internal energy
// SeeAlso
//   mousse::mixedFvPatchField
// SourceFiles
//   mixed_energy_fv_patch_scalar_field.cpp
#ifndef mixed_energy_fv_patch_scalar_field_hpp_
#define mixed_energy_fv_patch_scalar_field_hpp_
#include "mixed_fv_patch_fields.hpp"
namespace mousse
{
class mixedEnergyFvPatchScalarField
:
  public mixedFvPatchScalarField
{
public:
  //- Runtime type information
  TYPE_NAME("mixedEnergy");
  // Constructors
    //- Construct from patch and internal field
    mixedEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    mixedEnergyFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given mixedEnergyFvPatchScalarField
    // onto a new patch
    mixedEnergyFvPatchScalarField
    (
      const mixedEnergyFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    mixedEnergyFvPatchScalarField
    (
      const mixedEnergyFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new mixedEnergyFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    mixedEnergyFvPatchScalarField
    (
      const mixedEnergyFvPatchScalarField&,
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
        new mixedEnergyFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
};
}  // namespace mousse
#endif
