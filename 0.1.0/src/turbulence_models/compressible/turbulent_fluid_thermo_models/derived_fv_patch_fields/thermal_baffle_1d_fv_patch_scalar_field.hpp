#ifndef TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_THERMAL_BAFFLE_1D_FV_PATCH_SCALAR_FIELD_HPP_
#define TURBULENCE_MODELS_COMPRESSIBLE_TURBULENT_FLUID_THERMO_MODELS_DERIVED_FV_PATCH_FIELDS_THERMAL_BAFFLE_1D_FV_PATCH_SCALAR_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermalBaffle1DFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This BC solves a steady 1D thermal baffle. The solid properties are
//   specify as dictionary. Optionaly radiative heat flux (Qr) can be
//   incorporated into the balance. Some under-relaxation might be needed on
//   Qr.
//   Baffle and solid properties need to be specified on the master side
//   of the baffle.
//   \heading Patch usage
//   Example of the boundary condition specification using constant
//   solid thermo :
//   \verbatim
//   myPatch_master
//   {
//     type   compressible::thermalBaffle1D<hConstSolidThermoPhysics>;
//     samplePatch     myPatch_slave;
//     thickness       uniform 0.005;  // thickness [m]
//     Qs              uniform 100;    // heat flux [W/m2]
//     Qr              none;
//     relaxation      1;
//     // Solid thermo
//     specie
//     {
//       nMoles          1;
//       molWeight       20;
//     }
//     transport
//     {
//       kappa           1;
//     }
//     thermodynamics
//     {
//       Hf              0;
//       Cp              10;
//     }
//     equationOfState
//     {
//       rho             10;
//     }
//     value               uniform 300;
//   }
//   myPatch_slave
//   {
//     type   compressible::thermalBaffle1D<hConstSolidThermoPhysics>;
//     samplePatch     myPatch_master_master;
//     Qr              none;
//     relaxation      1;
//   }
//   \endverbatim
// SourceFiles
//   thermal_baffle_1d_fv_patch_scalar_field.cpp
#include "mixed_fv_patch_fields.hpp"
#include "auto_ptr.hpp"
#include "mapped_patch_base.hpp"
namespace mousse
{
namespace compressible
{
template<class solidType>
class thermalBaffle1DFvPatchScalarField
:
  public mappedPatchBase,
  public mixedFvPatchScalarField
{
  // Private data
    //- Name of the temperature field
    word TName_;
    //- Baffle is activated
    bool baffleActivated_;
    //- Baffle thickness [m]
    mutable scalarField thickness_;
    //- Superficial heat source [W/m2]
    mutable scalarField Qs_;
    //- Solid dictionary
    dictionary  solidDict_;
    //- Solid thermo
    mutable autoPtr<solidType> solidPtr_;
    //- Cache Qr for relaxation
    scalarField QrPrevious_;
    //- Relaxation for Qr
    scalar QrRelaxation_;
    //- Name of the radiative heat flux in local region
    const word QrName_;
  // Private members
    //- Return const solid thermo
    const solidType& solid() const;
    //- Return Qs from master
    tmp<scalarField> Qs() const;
    //- Return thickness from master
    tmp<scalarField> baffleThickness() const;
    //- Is Owner
    bool owner() const;
public:
  //- Runtime type information
  TYPE_NAME("compressible::thermalBaffle1D");
  // Constructors
    //- Construct from patch and internal field
    thermalBaffle1DFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    thermalBaffle1DFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  thermalBaffle1DFvPatchScalarField onto a new patch
    thermalBaffle1DFvPatchScalarField
    (
      const thermalBaffle1DFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct as copy
    thermalBaffle1DFvPatchScalarField
    (
      const thermalBaffle1DFvPatchScalarField&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new thermalBaffle1DFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    thermalBaffle1DFvPatchScalarField
    (
      const thermalBaffle1DFvPatchScalarField&,
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
        new thermalBaffle1DFvPatchScalarField(*this, iF)
      );
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
    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();
    //- Write
    virtual void write(Ostream&) const;
};
}  // namespace compressible
}  // namespace mousse
#ifdef NoRepository
#   include "thermal_baffle_1d_fv_patch_scalar_field.cpp"
#endif
#endif
