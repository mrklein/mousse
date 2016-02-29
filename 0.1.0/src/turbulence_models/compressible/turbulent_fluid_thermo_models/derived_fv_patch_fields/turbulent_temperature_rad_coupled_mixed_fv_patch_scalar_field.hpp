#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_TURBULENT_TEMPERATURE_RAD_COUPLED_MIXED_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_TURBULENT_TEMPERATURE_RAD_COUPLED_MIXED_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::compressible::
//     turbulentTemperatureRadCoupledMixedFvPatchScalarField
// Description
//   Mixed boundary condition for temperature and radiation heat transfer
//   to be used for in multiregion cases. Optional thin thermal layer
//   resistances can be specified through thicknessLayers and kappaLayers
//   entries.
//   The thermal conductivity, \c kappa, can either be retrieved from the
//   mesh database using the \c lookup option, or from a \c solidThermo
//   or \c fluidThermo thermophysical package.
//   \heading Patch usage
//   \table
//     Property     | Description             | Required    | Default value
//     kappa        | thermal conductivity option | yes     |
//     kappaName    | name of thermal conductivity field | no  |
//     Tnbr         | name of the field    | no | T
//     QrNbr      | name of the radiative flux in the nbr region | no | none
//     Qr         | name of the radiative flux in this region | no | none
//     thicknessLayers | list of thicknesses per layer [m] | no |
//     kappaLayers  | list of thermal conductivites per layer [W/m/K] | no |
//   \endtable
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch
//   {
//     type            compressible::turbulentTemperatureRadCoupledMixed;
//     Tnbr            T;
//     kappa           lookup;
//     KappaName       kappa;
//     QrNbr           Qr; // or none. Name of Qr field on neighbour region
//     Qr              Qr; // or none. Name of Qr field on local region
//     thicknessLayers (0.1 0.2 0.3 0.4);
//     kappaLayers     (1 2 3 4)
//     value           uniform 300;
//   }
//   \endverbatim
//   Needs to be on underlying mapped(Wall)FvPatch.
//   Note: kappa : heat conduction at patch. Gets supplied how to
//       lookup/calculate
//   kappa:
//   - 'lookup' : lookup volScalarField (or volSymmTensorField) with name
//   - 'fluidThermo' : use fluidThermo and compressible::RASmodel to calculate K
//   - 'solidThermo' : use solidThermo kappa()
//   - 'directionalSolidThermo' directionalKappa()
// SourceFiles
//   turbulent_temperature_rad_coupled_mixed_fv_patch_scalar_field.cpp
#include "mixed_fv_patch_fields.hpp"
#include "temperature_coupled_base.hpp"
#include "scalar_list.hpp"
namespace mousse
{
namespace compressible
{
class turbulentTemperatureRadCoupledMixedFvPatchScalarField
:
  public mixedFvPatchScalarField,
  public temperatureCoupledBase
{
  // Private data
    //- Name of field on the neighbour region
    const word TnbrName_;
    //- Name of the radiative heat flux in the neighbout region
    const word QrNbrName_;
    //- Name of the radiative heat flux in local region
    const word QrName_;
    //- Thickness of layers
    scalarList thicknessLayers_;
    //- Conductivity of layers
    scalarList kappaLayers_;
    //- Total contact resistance
    scalar contactRes_;
public:
  //- Runtime type information
  TYPE_NAME("compressible::turbulentTemperatureRadCoupledMixed");
  // Constructors
    //- Construct from patch and internal field
    turbulentTemperatureRadCoupledMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    turbulentTemperatureRadCoupledMixedFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  turbulentTemperatureCoupledBaffleMixedFvPatchScalarField onto a
    //  new patch
    turbulentTemperatureRadCoupledMixedFvPatchScalarField
    (
      const
      turbulentTemperatureRadCoupledMixedFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new turbulentTemperatureRadCoupledMixedFvPatchScalarField
        (
          *this
        )
      );
    }
    //- Construct as copy setting internal field reference
    turbulentTemperatureRadCoupledMixedFvPatchScalarField
    (
      const turbulentTemperatureRadCoupledMixedFvPatchScalarField&,
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
        new turbulentTemperatureRadCoupledMixedFvPatchScalarField
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
}  // namespace compressible
}  // namespace mousse
#endif
