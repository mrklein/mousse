// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentHeatFluxTemperatureFvPatchScalarField
// Description
//   Fixed heat boundary condition to specify temperature gradient. Input
//   heat source either specified in terms of an absolute power [W], or as a
//   flux [W/m^2].
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     heatSource   | 'power' [W] or 'flux' [W/m^2] | yes |
//     q            | heat power or flux field      | yes |
//     kappa        | inherited from temperatureCoupledBase | yes |
//     kappaName    | inherited from temperatureCoupledBase | yes |
//     Qr           | name of the radiative flux field | yes |
//     value        | initial temperature value | no | calculated
//     gradient     | initial gradient value | no | 0.0
//   \endtable
//   Note: If needed, both 'value' and 'gradient' must be defined to be used.
//   Example usage:
//   \verbatim
//   hotWall
//   {
//     type            compressible::turbulentHeatFluxTemperature;
//     heatSource      flux;
//     q               uniform 10;
//     kappa           fluidThermo;
//     kappaName       none;
//     Qr              none;
//   }
//   \endverbatim
// SeeAlso
//   mousse::temperatureCoupledBase
// SourceFiles
//   turbulent_heat_flux_temperature_fv_patch_scalar_field.cpp
#ifndef turbulentHeatFluxTemperatureFvPatchScalarFields_H
#define turbulentHeatFluxTemperatureFvPatchScalarFields_H
#include "fixed_gradient_fv_patch_fields.hpp"
#include "temperature_coupled_base.hpp"
namespace mousse
{
namespace compressible
{
class turbulentHeatFluxTemperatureFvPatchScalarField
:
  public fixedGradientFvPatchScalarField,
  public temperatureCoupledBase
{
public:
  // Data types
    //- Enumeration listing the possible hest source input modes
    enum heatSourceType
    {
      hsPower,
      hsFlux
    };
private:
  // Private data
    //- Heat source type names
    static const NamedEnum<heatSourceType, 2> heatSourceTypeNames_;
    //- Heat source type
    heatSourceType heatSource_;
    //- Heat power [W] or flux [W/m2]
    scalarField q_;
    //- Name of radiative in flux field
    word QrName_;
public:
  //- Runtime type information
  TYPE_NAME("compressible::turbulentHeatFluxTemperature");
  // Constructors
    //- Construct from patch and internal field
    turbulentHeatFluxTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentHeatFluxTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  turbulentHeatFluxTemperatureFvPatchScalarField onto
    //  a new patch
    turbulentHeatFluxTemperatureFvPatchScalarField
    (
      const turbulentHeatFluxTemperatureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    turbulentHeatFluxTemperatureFvPatchScalarField
    (
      const turbulentHeatFluxTemperatureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new turbulentHeatFluxTemperatureFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    turbulentHeatFluxTemperatureFvPatchScalarField
    (
      const turbulentHeatFluxTemperatureFvPatchScalarField&,
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
        new turbulentHeatFluxTemperatureFvPatchScalarField
        (
          *this,
          iF
        )
      );
    }
  // Member functions
    // Mapping functions
      //- Map (and resize as needed) from self given a mapping object
      virtual void autoMap(const fvPatchFieldMapper&);
      //- Reverse map the given fvPatchField onto this fvPatchField
      virtual void rmap
      (
        const fvPatchScalarField&,
        const labelList&
      );
    // Evaluation functions
      //- Update the coefficients associated with the patch field
      virtual void updateCoeffs();
    // I-O
      //- Write
      virtual void write(Ostream&) const;
};
}  // namespace compressible
}  // namespace mousse
#endif
