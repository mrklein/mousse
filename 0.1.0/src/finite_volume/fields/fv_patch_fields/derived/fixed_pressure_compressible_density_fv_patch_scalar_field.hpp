#ifndef FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_PRESSURE_COMPRESSIBLE_DENSITY_FV_PATCH_SCALAR_FIELD_HPP_
#define FINITE_VOLUME_FIELDS_FV_PATCH_FIELDS_DERIVED_FIXED_PRESSURE_COMPRESSIBLE_DENSITY_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedPressureCompressibleDensityFvPatchScalarField
// Group
//   grpInletBoundaryConditions
// Description
//   This boundary condition calculates a (liquid) compressible density as a
//   function of pressure and fluid properties:
//     \f[
//       \rho = \rho_{l,sat} + \psi_l*(p - p_{sat})
//     \f]
//   where
//   \vartable
//     \rho    | density [kg/m3]
//     \rho_{l,sat} | saturation liquid density [kg/m3]
//     \psi_l  | liquid compressibility
//     p       | pressure [Pa]
//     p_{sat} | saturation pressure [Pa]
//   \endvartable
//   The variables \c rholSat, \c pSat and \c psil are retrieved from the
//   \c thermodynamicProperties dictionary.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     p            | pressure field name     | no          | p
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type        fixedPressureCompressibleDensity;
//     p           p;
//     value       uniform 1;
//   }
//   \endverbatim
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   fixed_pressure_compressible_density_fv_patch_scalar_field.cpp
#include "fixed_value_fv_patch_fields.hpp"
namespace mousse
{
class fixedPressureCompressibleDensityFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
  // Private data
    //- Name of static pressure field
    word pName_;
public:
  //- Runtime type information
  TYPE_NAME("fixedPressureCompressibleDensity");
  // Constructors
    //- Construct from patch and internal field
    fixedPressureCompressibleDensityFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    fixedPressureCompressibleDensityFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  fixedPressureCompressibleDensityFvPatchScalarField
    //  onto a new patch
    fixedPressureCompressibleDensityFvPatchScalarField
    (
      const fixedPressureCompressibleDensityFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    fixedPressureCompressibleDensityFvPatchScalarField
    (
      const fixedPressureCompressibleDensityFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new fixedPressureCompressibleDensityFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    fixedPressureCompressibleDensityFvPatchScalarField
    (
      const fixedPressureCompressibleDensityFvPatchScalarField&,
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
        new fixedPressureCompressibleDensityFvPatchScalarField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
