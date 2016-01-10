// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::waveSurfacePressureFvPatchScalarField
// Group
//   grpInletBoundaryConditions
// Description
//   This is a pressure boundary condition, whose value is calculated as
//   the hydrostatic pressure based on a given displacement:
//     \f[
//       p = -\rho*g*\zeta
//     \f]
//   \vartable
//     \rho  | density [kg/m3]
//     g     | acceleration due to gravity [m/s2]
//     \zeta | wave amplitude [m]
//   \endvartable
//   The wave amplitude is updated as part of the calculation, derived from the
//   local volumetric flux.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     phi          | flux field name         | no          | phi
//     rho          | density field name      | no          | rho
//     zeta         | wave amplitude field name | no        | zeta
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            waveSurfacePressure;
//     phi             phi;
//     rho             rho;
//     zeta            zeta;
//     value           uniform 0;  // place holder
//   }
//   \endverbatim
//   The density field is only required if the flux is mass-based as opposed to
//   volumetric-based.
// SeeAlso
//   mousse::fixedValueFvPatchField
// SourceFiles
//   wave_surface_pressure_fv_patch_scalar_field.cpp
#ifndef wave_surface_pressure_fv_patch_scalar_field_hpp_
#define wave_surface_pressure_fv_patch_scalar_field_hpp_
#include "fixed_value_fv_patch_fields.hpp"
#include "named_enum.hpp"
namespace mousse
{
class waveSurfacePressureFvPatchScalarField
:
  public fixedValueFvPatchScalarField
{
public:
  // Public data
    //- Enumeration defining the available ddt schemes
    enum ddtSchemeType
    {
      tsEuler,
      tsCrankNicolson,
      tsBackward
    };
private:
  // Private data
    //- Flux field name
    word phiName_;
    //- Wave height field name
    word zetaName_;
    //- Density field for mass-based flux evaluations
    word rhoName_;
    //- Time scheme type names
    static const NamedEnum<ddtSchemeType, 3> ddtSchemeTypeNames_;
public:
  //- Runtime type information
  TYPE_NAME("waveSurfacePressure");
  // Constructors
    //- Construct from patch and internal field
    waveSurfacePressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    waveSurfacePressureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given waveSurfacePressureFvPatchScalarField
    //  onto a new patch
    waveSurfacePressureFvPatchScalarField
    (
      const waveSurfacePressureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    waveSurfacePressureFvPatchScalarField
    (
      const waveSurfacePressureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new waveSurfacePressureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    waveSurfacePressureFvPatchScalarField
    (
      const waveSurfacePressureFvPatchScalarField&,
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
        new waveSurfacePressureFvPatchScalarField(*this, iF)
      );
    }
  // Member functions
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace mousse
#endif
