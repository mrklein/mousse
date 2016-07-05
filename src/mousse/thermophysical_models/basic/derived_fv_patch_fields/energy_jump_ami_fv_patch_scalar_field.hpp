#ifndef THERMOPHYSICAL_MODELS_BASIC_DERIVED_FV_PATCH_FIELDS_ENERGY_JUMP_AMI_FV_PATCH_SCALAR_FIELD_HPP_
#define THERMOPHYSICAL_MODELS_BASIC_DERIVED_FV_PATCH_FIELDS_ENERGY_JUMP_AMI_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyJumpAMIFvPatchScalarField
// Group
//   grpThermoBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition provides an energy jump condition across a pair
//   of coupled patches with an arbitrary mesh interface (AMI).  It is not
//   applied directly, but is employed on-the-fly when converting temperature
//   boundary conditions into energy.
// SeeAlso
//   mousse::fixedJumpAMIFvPatchField

#include "fixed_jump_ami_fv_patch_field.hpp"


namespace mousse {

class energyJumpAMIFvPatchScalarField
:
  public fixedJumpAMIFvPatchField<scalar>
{
public:
  //- Runtime type information
  TYPE_NAME("energyJumpAMI");
  // Constructors
    //- Construct from patch and internal field
    energyJumpAMIFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    energyJumpAMIFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given energyJumpAMIFvPatchScalarField onto a
    //  new patch
    energyJumpAMIFvPatchScalarField
    (
      const energyJumpAMIFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    energyJumpAMIFvPatchScalarField
    (
      const energyJumpAMIFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar>> clone() const
    {
      return
        tmp<fvPatchField<scalar>>
        {
          new energyJumpAMIFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    energyJumpAMIFvPatchScalarField
    (
      const energyJumpAMIFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar>> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchField<scalar>>
        {
          new energyJumpAMIFvPatchScalarField{*this, iF}
        };
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};

}  // namespace mousse

#endif

