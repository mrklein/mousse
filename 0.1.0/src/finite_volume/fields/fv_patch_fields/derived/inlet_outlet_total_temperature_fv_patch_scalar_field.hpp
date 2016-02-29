#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_INLET_OUTLET_TOTAL_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_INLET_OUTLET_TOTAL_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::inletOutletTotalTemperatureFvPatchScalarField
// Group
//   grpOutletBoundaryConditions
// Description
//   This boundary condition provides an outflow condition for total
//   temperature for use with supersonic cases, where a user-specified
//   value is applied in the case of reverse flow.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velocity field name     | no          | U
//     phi          | flux field name         | no          | phi
//     psi          | compressibility field name |  no      | psi
//     gamma        | heat capacity ration (Cp/Cv) | yes    |
//     inletValue   | reverse flow (inlet) value | yes      |
//     T0           | static temperature [K]  | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            inletOutletTotalTemperature;
//     U               U;
//     phi             phi;
//     psi             psi;
//     gamma           gamma;
//     inletValue      uniform 0;
//     T0              uniform 0;
//     value           uniform 0;
//   }
//   \endverbatim
// SeeAlso
//   mousse::inletOutletFvPatchField
// SourceFiles
//   inlet_outlet_total_temperature_fv_patch_scalar_field.cpp
#include "inlet_outlet_fv_patch_fields.hpp"
namespace mousse
{
class inletOutletTotalTemperatureFvPatchScalarField
:
  public inletOutletFvPatchScalarField
{
  // Private data
    //- Name of the velocity field
    word UName_;
    //- Name of the compressibility field used to calculate the wave speed
    word psiName_;
    //- Heat capacity ratio
    scalar gamma_;
    //- Total pressure
    scalarField T0_;
public:
  //- Runtime type information
  TYPE_NAME("inletOutletTotalTemperature");
  // Constructors
    //- Construct from patch and internal field
    inletOutletTotalTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    inletOutletTotalTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  inletOutletTotalTemperatureFvPatchScalarField onto a new patch
    inletOutletTotalTemperatureFvPatchScalarField
    (
      const inletOutletTotalTemperatureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    inletOutletTotalTemperatureFvPatchScalarField
    (
      const inletOutletTotalTemperatureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new inletOutletTotalTemperatureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    inletOutletTotalTemperatureFvPatchScalarField
    (
      const inletOutletTotalTemperatureFvPatchScalarField&,
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
        new inletOutletTotalTemperatureFvPatchScalarField(*this, iF)
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
