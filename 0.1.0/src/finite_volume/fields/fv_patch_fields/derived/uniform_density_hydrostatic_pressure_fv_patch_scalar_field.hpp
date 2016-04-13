#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_DENSITY_HYDROSTATIC_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_UNIFORM_DENSITY_HYDROSTATIC_PRESSURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniformDensityHydrostaticPressureFvPatchScalarField
// Group
//   grpGenericBoundaryConditions
// Description
//   This boundary condition provides a hydrostatic pressure condition,
//   calculated as:
//     \f[
//       p_{hyd} = p_{ref} + \rho g (x - x_{ref})
//     \f]
//   where
//   \vartable
//     p_{hyd} | hyrostatic pressure [Pa]
//     p_{ref} | reference pressure [Pa]
//     x_{ref} | reference point in Cartesian co-ordinates
//     \rho    | density (assumed uniform)
//     g       | acceleration due to gravity [m/s2]
//   \endtable
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     rho          | uniform density [kg/m3] | yes         |
//     pRefValue    | reference pressure [Pa] | yes         |
//     pRefPoint    | reference pressure location | yes     |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            uniformDensityHydrostaticPressure;
//     rho             rho;
//     pRefValue       1e5;
//     pRefPoint       (0 0 0);
//     value           uniform 0; // optional initial value
//   }
//   \endverbatim

#include "fixed_value_fv_patch_fields.hpp"


namespace mousse {

class uniformDensityHydrostaticPressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Constant density in the far-field
    scalar rho_;
    //- Reference pressure
    scalar pRefValue_;
    //- Reference pressure location
    vector pRefPoint_;
public:
  //- Runtime type information
  TYPE_NAME("uniformDensityHydrostaticPressure");
  // Constructors
    //- Construct from patch and internal field
    uniformDensityHydrostaticPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    uniformDensityHydrostaticPressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  uniformDensityHydrostaticPressureFvPatchScalarField onto a new patch
    uniformDensityHydrostaticPressureFvPatchScalarField
    (
      const uniformDensityHydrostaticPressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    uniformDensityHydrostaticPressureFvPatchScalarField
    (
      const uniformDensityHydrostaticPressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      {
        new uniformDensityHydrostaticPressureFvPatchScalarField{*this}
      };
    }
    //- Construct as copy setting internal field reference
    uniformDensityHydrostaticPressureFvPatchScalarField
    (
      const uniformDensityHydrostaticPressureFvPatchScalarField&,
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
        new uniformDensityHydrostaticPressureFvPatchScalarField{*this, iF}
      };
    }
  // Member functions
    // Access
      //- Return the constant density in the far-field
      scalar rho() const
      {
        return rho_;
      }
      //- Return reference to the constant density in the far-field
      //  to allow adjustment
      scalar& rho()
      {
        return rho_;
      }
      //- Return the reference pressure
      scalar pRefValue() const
      {
        return pRefValue_;
      }
      //- Return reference to the reference pressure to allow adjustment
      scalar& pRefValue()
      {
        return pRefValue_;
      }
      //- Return the pressure reference location
      const vector& pRefPoint() const
      {
        return pRefPoint_;
      }
      //- Return reference to the pressure reference location
      //  to allow adjustment
      vector& pRefPoint()
      {
        return pRefPoint_;
      }
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
