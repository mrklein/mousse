// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::energyJumpFvPatchScalarField
// Group
//   grpThermoBoundaryConditions grpCoupledBoundaryConditions
// Description
//   This boundary condition provides an energy jump condition across a pair
//   of coupled patches.  It is not applied directly, but is employed on-the-fly
//   when converting temperature boundary conditions into energy.
// SeeAlso
//   mousse::fixedJumpFvPatchField
// SourceFiles
//   energy_jump_fv_patch_scalar_field.cpp
#ifndef energy_jump_fv_patch_scalar_field_hpp_
#define energy_jump_fv_patch_scalar_field_hpp_
#include "fixed_jump_fv_patch_field.hpp"
#include "data_entry.hpp"
namespace mousse
{
class energyJumpFvPatchScalarField
:
  public fixedJumpFvPatchField<scalar>
{
public:
  //- Runtime type information
  TYPE_NAME("energyJump");
  // Constructors
    //- Construct from patch and internal field
    energyJumpFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    energyJumpFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given energyJumpFvPatchScalarField onto a
    //  new patch
    energyJumpFvPatchScalarField
    (
      const energyJumpFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    energyJumpFvPatchScalarField
    (
      const energyJumpFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchField<scalar> > clone() const
    {
      return tmp<fvPatchField<scalar> >
      (
        new energyJumpFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    energyJumpFvPatchScalarField
    (
      const energyJumpFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchField<scalar> > clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchField<scalar> >
      (
        new energyJumpFvPatchScalarField(*this, iF)
      );
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
