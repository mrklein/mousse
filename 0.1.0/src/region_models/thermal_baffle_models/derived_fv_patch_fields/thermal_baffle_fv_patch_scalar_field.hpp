// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermalBaffleFvPatchScalarField
// Group
//   grpThermoBoundaryConditions
// Description
//   This boundary condition provides a coupled temperature condition between
//   multiple mesh regions.  The regions are generally referred to as the:
//   - primary region, and
//   - baffle region
//   The primary region creates the baffle region and evolves its energy
//   equation either:
//   - 1-D, normal to each patch face
//   - 2-D, normal and tangential components
//   The thermodynamic properties of the baffle material are specified via
//   dictionary entries on the master patch
//   \heading Patch usage
//   Example of the boundary condition specification:
//   \verbatim
//   myPatch_master
//   {
//     type                compressible::thermalBaffle;
//     // Underlaying coupled boundary condition
//     Tnbr               T;
//     kappa              fluidThermo; // or solidThermo
//     KappaName          none;
//     QrNbr              Qr;//or none.Name of Qr field on neighbourregion
//     Qr                 none;// or none.Name of Qr field on localregion
//     value              uniform 300;
//     // Baffle region name
//     regionName          baffleRegion;
//     active              yes;
//     // Solid thermo in solid region
//     thermoType
//     {
//       type            heSolidThermo;
//       mixture         pureMixture;
//       transport       constIso;
//       thermo          hConst;
//       equationOfState rhoConst;
//       specie          specie;
//       energy          sensibleEnthalpy;
//     }
//     mixture
//     {
//       specie
//       {
//         nMoles          1;
//         molWeight       20;
//       }
//       transport
//       {
//         kappa           0.01;
//       }
//       thermodynamics
//       {
//         Hf              0;
//         Cp              15;
//       }
//       density
//       {
//         rho             80;
//       }
//     }
//     radiation
//     {
//       radiationModel  opaqueSolid;
//       absorptionEmissionModel none;
//       scatterModel    none;
//     }
//     // Extrude model for new region
//     extrudeModel        linearNormal;
//     nLayers             50;
//     expansionRatio      1;
//     columnCells         false; //3D or 1D
//     linearNormalCoeffs
//     {
//       thickness           0.02;
//     }
//   }
//   \endverbatim
//   /- Slave patch on primary region
//   \verbatim
//   myPatch_slave
//   {
//     type                compressible::thermalBaffle;
//     kappa               fluidThermo;
//     kappaName           none;
//     value               uniform 300;
//   \endverbatim
//   /- Patches on baffle region
//   \verbatim
//   bottom
//   {
//     type                compressible::thermalBaffle;
//     kappa               solidThermo;
//     kappaName           none;
//     value               uniform 300;
//   }
//   top
//   {
//     type                compressible::thermalBaffle;
//     kappa               solidThermo;
//     kappaName           none;
//     value               uniform 300;
//   }
//   \endverbatim
// SeeAlso
//   mousse::turbulentTemperatureCoupledBaffleMixedFvPatchScalarField
//   mousse::regionModels::thermalBaffleModels::thermalBaffleModel
// SourceFiles
//   thermal_baffle_fv_patch_scalar_field.cpp
#ifndef thermal_baffle_fv_patch_scalar_field_hpp_
#define thermal_baffle_fv_patch_scalar_field_hpp_
#include "auto_ptr.hpp"
#include "region_model.hpp"
#include "thermal_baffle_model.hpp"
#include "extrude_patch_mesh.hpp"
#include "turbulent_temperature_rad_coupled_mixed_fv_patch_scalar_field.hpp"
namespace mousse
{
namespace compressible
{
class thermalBaffleFvPatchScalarField
:
  public turbulentTemperatureRadCoupledMixedFvPatchScalarField
{
  // Private data
    //- Enumeration of patch IDs
    enum patchID
    {
      bottomPatchID,
      topPatchID,
      sidePatchID
    };
    //- Is the baffle owner
    bool owner_;
    //- Thermal baffle
    autoPtr<regionModels::thermalBaffleModels::thermalBaffleModel> baffle_;
    //- Dictionary
    dictionary dict_;
    //- Auto pointer to extrapolated mesh from patch
    autoPtr<extrudePatchMesh> extrudeMeshPtr_;
  // Private member functions
    //- Extrude mesh
    void createPatchMesh();
public:
  //- Runtime type information
  TypeName("compressible::thermalBaffle");
  // Constructors
    //- Construct from patch and internal field
    thermalBaffleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&
    );
    //- Construct from patch, internal field and dictionary
    thermalBaffleFvPatchScalarField
    (
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const dictionary&
    );
    //- Construct by mapping given
    //  thermalBaffleFvPatchScalarField onto a new patch
    thermalBaffleFvPatchScalarField
    (
      const thermalBaffleFvPatchScalarField&,
      const fvPatch&,
      const DimensionedField<scalar, volMesh>&,
      const fvPatchFieldMapper&
    );
    //- Construct and return a clone
    virtual tmp<fvPatchScalarField> clone() const
    {
      return tmp<fvPatchScalarField>
      (
        new thermalBaffleFvPatchScalarField(*this)
      );
    }
    //- Construct as copy setting internal field reference
    thermalBaffleFvPatchScalarField
    (
      const thermalBaffleFvPatchScalarField&,
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
        new thermalBaffleFvPatchScalarField(*this, iF)
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
#endif
