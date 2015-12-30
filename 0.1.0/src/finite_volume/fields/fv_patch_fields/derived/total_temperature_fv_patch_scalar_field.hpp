// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::totalTemperatureFvPatchScalarField
// Group
//   grpInletBoundaryConditions grpOutletBoundaryConditions
// Description
//   This boundary condition provides a total temperature condition.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | Velocity field name     | no          | U
//     phi          | Flux field name         | no          | phi
//     psi          | Compressibility field name | no       | thermo:psi
//     gamma        | ratio of specific heats (Cp/Cv) | yes |
//     T0           | reference temperature   | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            totalTemperature;
//     T0              uniform 300;
//   }
//   \endverbatim
// SourceFiles
//   total_temperature_fv_patch_scalar_field.cpp
//   see_also
//   foam::fixed_value_fv_patch_field
#ifndef total_temperature_fv_patch_scalar_field_hpp_
#define total_temperature_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class totalTemperatureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Name of the velocity field
    word UName_;
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of the compressibility field used to calculate the wave speed
    word psiName_;
    //- Heat capacity ratio
    scalar gamma_;
    //- Total pressure
    scalarField T0_;
public:
  //- Runtime type information
  TypeName("totalTemperature");
  // Constructors
    //- Construct from patch and internal field
    totalTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    totalTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given totalTemperatureFvPatchScalarField
    //  onto a new patch
    totalTemperatureFvPatchScalarField
    (
      const totalTemperatureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    totalTemperatureFvPatchScalarField
    (
      const totalTemperatureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new totalTemperatureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    totalTemperatureFvPatchScalarField
    (
      const totalTemperatureFvPatchScalarField&,
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
        new totalTemperatureFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Access
      //- Return the total pressure
      const scalarField& T0() const
      {
        return T0_;
      }
      //- Return reference to the total pressure to allow adjustment
      scalarField& T0()
      {
        return T0_;
      }
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap
      (
        const fvPatchFieldMapper&
      );
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
