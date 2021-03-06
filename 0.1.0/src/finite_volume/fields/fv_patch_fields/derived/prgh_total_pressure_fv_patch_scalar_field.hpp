#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRGH_TOTAL_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_PRGH_TOTAL_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::prghTotalPressureFvPatchScalarField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition provides static pressure condition for p_rgh,
//   calculated as:
//     \f[
//       p_rgh = p - \rho g (h - hRef)
//       p = p0 - 0.5 \rho |U|^2
//     \f]
//   where
//   \vartable
//     p_rgh   | Pseudo hydrostatic pressure [Pa]
//     p       | Static pressure [Pa]
//     p0      | Total pressure [Pa]
//     h       | Height in the opposite direction to gravity
//     hRef    | Reference height in the opposite direction to gravity
//     \rho    | density
//     g       | acceleration due to gravity [m/s^2]
//   \endtable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     U            | velocity field name     | no          | U
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     p0           | total pressure          | yes         |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            prghTotalPressure;
//     U               U;
//     phi             phi;
//     rho             rho;
//     p0              uniform 0;
//     value           uniform 0; // optional initial value
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchScalarField

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class prghTotalPressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
protected:
  // Protected data
    //- Name of the velocity field
    word UName_;
    //- Name of the flux transporting the field
    word phiName_;
    //- Name of phase-fraction field
    word rhoName_;
    //- Total pressure
    scalarField p0_;
public:
  //- Runtime type information
  TYPE_NAME("prghTotalPressure");
  // Constructors
    //- Construct from patch and internal field
    prghTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    prghTotalPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  prghTotalPressureFvPatchScalarField onto a new patch
    prghTotalPressureFvPatchScalarField
    (
      const prghTotalPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    prghTotalPressureFvPatchScalarField
    (
      const prghTotalPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField >
      {
        new prghTotalPressureFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    prghTotalPressureFvPatchScalarField
    (
      const prghTotalPressureFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return tmp<fvPatchScalarField>
      {
        new prghTotalPressureFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the total pressure
      const scalarField& p0() const
      {
        return p0_;
      }
      //- Return reference to the total pressure to allow adjustment
      scalarField& p0()
      {
        return p0_;
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
