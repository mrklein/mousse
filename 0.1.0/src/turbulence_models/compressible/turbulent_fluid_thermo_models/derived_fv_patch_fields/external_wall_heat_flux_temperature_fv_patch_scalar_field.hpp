#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_EXTERNAL_WALL_HEAT_FLUX_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_EXTERNAL_WALL_HEAT_FLUX_TEMPERATURE_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::externalWallHeatFluxTemperatureFvPatchScalarField
// Group
//   grpThermoBoundaryConditions grpWallBoundaryConditions
// Description
//   This boundary condition supplies a heat flux condition for temperature
//   on an external wall. Optional thin thermal layer resistances can be
//   specified through thicknessLayers and kappaLayers entries for the
//   fixed heat transfer coefficient mode.
//   The condition can operate in two modes:
//   - fixed heat transfer coefficient: supply h and Ta
//   - fixed heat flux: supply q
//   where:
//   \vartable
//     h  | heat transfer coefficient [W/m^2/K]
//     Ta | ambient temperature [K]
//     q  | heat flux [W/m^2]
//   \endvartable
//   The thermal conductivity, \c kappa, can either be retrieved from the
//   mesh database using the \c lookup option, or from a \c solidThermo
//   thermophysical package.
//   \heading Patch usage
//   \table
//     Property     | Description                 | Required | Default value
//     kappa        | thermal conductivity option | yes      |
//     q            | heat flux [W/m^2]           | yes*     |
//     Ta           | ambient temperature [K]     | yes*     |
//     h            | heat transfer coefficient [W/m^2/K] | yes*|
//     thicknessLayers | list of thicknesses per layer [m] | yes |
//     kappaLayers  | list of thermal conductivites per layer [W/m/K] | yes |
//     kappaName    | name of thermal conductivity field | yes  |
//     Qr           | name of the radiative field | no | no
//     relaxation   | relaxation factor for radiative field | no | 1
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            externalWallHeatFluxTemperature;
//     kappa           fluidThermo;
//     q               uniform 1000;
//     Ta              uniform 300.0;
//     h               uniform 10.0;
//     thicknessLayers (0.1 0.2 0.3 0.4);
//     kappaLayers     (1 2 3 4);
//     value           uniform 300.0;
//     kappaName       none;
//     Qr              none;
//     relaxation      1;
//   }
//   \endverbatim
// Note
//  - Only supply \c h and \c Ta, or \c q in the dictionary (see above)
//  - kappa entries can be: fluidThermo, solidThermo or lookup

#include "mixed_fv_patch_fields.hpp"
#include "temperature_coupled_base.hpp"


namespace mousse {

class externalWallHeatFluxTemperatureFvPatchScalarField
:
  public mixedFvPatchScalarField,
  public temperatureCoupledBase
{
public:
  // Public data
    //- Operation mode enumeration
    enum operationMode
    {
      fixedHeatFlux,
      fixedHeatTransferCoeff,
      unknown
    };
    static const NamedEnum<operationMode, 3> operationModeNames;
private:
  // Private data
    //- Operation mode
    operationMode mode_;
    //- Heat flux / [W/m2]
    scalarField q_;
    //- Heat transfer coefficient / [W/m2K]
    scalarField h_;
    //- Ambient temperature / [K]
    scalarField Ta_;
    //- Chache Qr for relaxation
    scalarField QrPrevious_;
    //- Relaxation for Qr
    scalar QrRelaxation_;
    //- Name of the radiative heat flux
    const word QrName_;
    //- Thickness of layers
    scalarList thicknessLayers_;
    //- Conductivity of layers
    scalarList kappaLayers_;
public:
  //- Runtime type information
  TYPE_NAME("externalWallHeatFluxTemperature");
  // Constructors
    //- Construct from patch and internal field
    externalWallHeatFluxTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    externalWallHeatFluxTemperatureFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    // externalWallHeatFluxTemperatureFvPatchScalarField
    // onto a new patch
    externalWallHeatFluxTemperatureFvPatchScalarField
    (
      const externalWallHeatFluxTemperatureFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    externalWallHeatFluxTemperatureFvPatchScalarField
    (
      const externalWallHeatFluxTemperatureFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return
        tmp<fvPatchScalarField>
        {
          new externalWallHeatFluxTemperatureFvPatchScalarField{*this}
        };
    }
    //- Construct as copy setting internal field reference
    externalWallHeatFluxTemperatureFvPatchScalarField
    (
      const externalWallHeatFluxTemperatureFvPatchScalarField&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct and return a clone setting internal field reference
    virtual tmp<fvPatchScalarField> clone
    (
      const DimensionedField<scalar, volMesh>& iF
    ) const
    {
      return
        tmp<fvPatchScalarField>
        {
          new externalWallHeatFluxTemperatureFvPatchScalarField{*this, iF}
        };
      }
  // Member functions
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
    // I-O
      //- Write
      void write(Ostream&) const;
};

}  // namespace mousse

#endif

