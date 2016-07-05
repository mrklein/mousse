#ifndef REGION_MODELS_SURFACE_FILM_MODELS_THERMO_SINGLE_LAYER_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_THERMO_SINGLE_LAYER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermoSingleLayer
// Description
//   Thermodynamic form of single-cell layer surface film model
//   Note: defining enthalpy as Cp(T - Tstd) - when using liquids from the
//   thermophysical library, their enthalpies are calculated similarly, where
//   Tstd = 298.15 K

#include "kinematic_single_layer.hpp"
#include "slg_thermo.hpp"
#include "film_radiation_model.hpp"
#include "heat_transfer_model.hpp"
#include "phase_change_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Forward declaration of classes
class filmViscosityModel;
class heatTransferModel;
class phaseChangeModel;
class filmRadiationModel;


class thermoSingleLayer
:
  public kinematicSingleLayer
{
private:
  // Private member functions
    //- Return boundary types for sensible enthalpy field
    wordList hsBoundaryTypes();
protected:
  // Protected data
    // Thermo properties
      //- Reference to the SLGThermo
      const SLGThermo& thermo_;
      // Fields
        //- Specific heat capacity / [J/kg/K]
        volScalarField Cp_;
        //- Thermal conductivity / [W/m/K]
        volScalarField kappa_;
        //- Temperature - mean / [K]
        volScalarField T_;
        //- Temperature - surface / [K]
        volScalarField Ts_;
        //- Temperature - wall / [K]
        volScalarField Tw_;
        //- Sensible enthalpy / [J/kg]
        volScalarField hs_;
      // Transfer fields - to the primary region
        //- Film mass evolved via phase change
        volScalarField primaryMassPCTrans_;
        //- Film energy evolved via phase change
        volScalarField primaryEnergyPCTrans_;
    //- Threshold film thickness beyond which the film is considered 'wet'
    scalar deltaWet_;
    // Hyprophilic/phobic properties
      //- Activation flag
      bool hydrophilic_;
      //- Length scale applied to deltaWet_ to determine when a wet
      //  surface becomes dry, typically 0.5
      scalar hydrophilicDryScale_;
      //- Length scale applied to deltaWet_ to determine when a dry
      //  surface becomes wet, typically 0.001
      scalar hydrophilicWetScale_;
    // Source term fields
      // Film region - registered to the film region mesh
      // Note: need boundary value mapped from primary region, and then
      // pushed into the patch internal field
        //- Energy / [J/m2/s]
        volScalarField hsSp_;
      // Primary region - registered to the primary region mesh
      // Internal use only - not read-in
        //- Energy / [J/m2/s]
        volScalarField hsSpPrimary_;
    // Fields mapped from primary region - registered to the film region
    // Note: need both boundary AND patch internal fields to be mapped
      //- Temperature / [K]
      volScalarField TPrimary_;
      //- List of specie mass fractions / [0-1]
      PtrList<volScalarField> YPrimary_;
    // Sub-models
      //- Viscosity model
      autoPtr<filmViscosityModel> viscosity_;
      //- Heat transfer coefficient bewteen film surface and primary
      //  region [W/m2/K]
      autoPtr<heatTransferModel> htcs_;
      //- Heat transfer coefficient bewteen wall and film [W/m2/K]
      autoPtr<heatTransferModel> htcw_;
      //- Phase change
      autoPtr<phaseChangeModel> phaseChange_;
      //- Radiation
      autoPtr<filmRadiationModel> radiation_;
    // Limits
      //- Minimum temperature limit (optional)
      scalar Tmin_;
      //- Maximum temperature limit (optional)
      scalar Tmax_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
    //- Correct the thermo fields
    virtual void correctThermoFields();
    //- Correct sensible enthalpy for mapped temperature fields
    virtual void correctHsForMappedT();
    //- Correct the film surface and wall temperatures
    virtual void updateSurfaceTemperatures();
    //- Reset source term fields
    virtual void resetPrimaryRegionSourceTerms();
    //- Transfer thermo fields from the primary region to the film region
    virtual void transferPrimaryRegionThermoFields();
    //- Transfer source fields from the primary region to the film region
    virtual void transferPrimaryRegionSourceFields();
    //- Correct film coverage field
    virtual void correctAlpha();
    //- Update the film sub-models
    virtual void updateSubmodels();
    //- Return the wall/surface heat transfer term for the enthalpy equation
    virtual tmp<fvScalarMatrix> q(volScalarField& hs) const;
    // Equations
      //- Solve energy equation
      virtual void solveEnergy();
public:
  //- Runtime type information
  TYPE_NAME("thermoSingleLayer");
  // Constructors
    //- Construct from components
    thermoSingleLayer
    (
      const word& modelType,
      const fvMesh& mesh,
      const dimensionedVector& g,
      const word& regionType,
      const bool readFields = true
    );
    //- Disallow default bitwise copy construct
    thermoSingleLayer(const thermoSingleLayer&) = delete;
    //- Disallow default bitwise assignment
    thermoSingleLayer& operator=(const thermoSingleLayer&) = delete;
  //- Destructor
  virtual ~thermoSingleLayer();
  // Member Functions
    // Thermo properties
      //- Return const reference to the SLGThermo object
      inline const SLGThermo& thermo() const;
      // Fields
        //- Return the film specific heat capacity [J/kg/K]
        virtual const volScalarField& Cp() const;
        //- Return the film thermal conductivity [W/m/K]
        virtual const volScalarField& kappa() const;
        //- Return the film mean temperature [K]
        virtual const volScalarField& T() const;
        //- Return the film surface temperature [K]
        virtual const volScalarField& Ts() const;
        //- Return the film wall temperature [K]
        virtual const volScalarField& Tw() const;
        //- Return the film sensible enthalpy [J/kg]
        virtual const volScalarField& hs() const;
      // Transfer fields - to the primary region
        //- Return mass transfer source - Eulerian phase only
        virtual tmp<volScalarField> primaryMassTrans() const;
      // Helper functions
        //- Return sensible enthalpy as a function of temperature
        //  for a patch
        inline tmp<scalarField> hs
        (
          const scalarField& T,
          const label patchI
        ) const;
        //- Return sensible enthalpy as a function of temperature
        inline tmp<volScalarField> hs
        (
          const volScalarField& T
        ) const;
        //- Return temperature as a function of sensible enthalpy
        inline tmp<volScalarField> T
        (
          const volScalarField& hs
        ) const;
    // Source fields (read/write access)
      //- External hook to add sources to the film
      virtual void addSources
      (
        const label patchI,            // patchI on primary region
        const label faceI,             // faceI of patchI
        const scalar massSource,       // [kg]
        const vector& momentumSource,  // [kg.m/s] (tangential momentum)
        const scalar pressureSource,   // [kg.m/s] (normal momentum)
        const scalar energySource      // [J]
      );
    // Source term fields
      // Film region
        //- Energy / [J/m2/s]
        inline const volScalarField& hsSp() const;
      // Primary region
        //- Energy / [J/m2/s]
        inline const volScalarField& hsSpPrimary() const;
    // Fields mapped from the primary region
      //- Temperature / [K]
      inline const volScalarField& TPrimary() const;
      //- Specie mass fractions / [0-1]
      inline const PtrList<volScalarField>& YPrimary() const;
    // Sub-models
      //- Return const access to the (surface) heat transfer model
      inline const heatTransferModel& htcs() const;
      //- Return const access to the (wall) heat transfer model
      inline const heatTransferModel& htcw() const;
      //- Return const access to the phase change model
      inline const phaseChangeModel& phaseChange() const;
      //- Return const access to the radiation model
      inline const filmRadiationModel& radiation() const;
    // Derived fields (calculated on-the-fly)
      //- Return the convective heat energy from film to wall
      inline tmp<scalarField> Qconvw(const label patchI) const;
      //- Return the convective heat energy from primary region to film
      inline tmp<scalarField> Qconvp(const label patchI) const;
    // Evolution
      //- Pre-evolve film hook
      virtual void preEvolveRegion();
      //- Evolve the film equations
      virtual void evolveRegion();
    // Source fields
      // Mapped into primary region
        //- Return total mass source - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Srho() const;
        //- Return mass source for specie i - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Srho
        (
          const label i
        ) const;
        //- Return enthalpy source - Eulerian phase only
        virtual tmp<DimensionedField<scalar, volMesh> > Sh() const;
   // I-O
      //- Provide some feedback
      virtual void info();
};


inline const SLGThermo& thermoSingleLayer::thermo() const
{
  return thermo_;
}


inline tmp<scalarField> thermoSingleLayer::hs
(
  const scalarField& T,
  const label patchI
) const
{
  const scalarField& Cp = Cp_.boundaryField()[patchI];
  return Cp*(T - 298.15);
}


inline tmp<volScalarField> thermoSingleLayer::hs
(
  const volScalarField& T
) const
{
  return tmp<volScalarField>
  {
    new volScalarField
    {
      {
        "hs(" + T.name() + ")",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      Cp_*(T - (dimensionedScalar{"Tstd", dimTemperature, 298.15})),
      zeroGradientFvPatchScalarField::typeName
    }
  };
}


inline tmp<volScalarField> thermoSingleLayer::T
(
  const volScalarField& hs
) const
{
  tmp<volScalarField> tT
  {
    new volScalarField
    {
      {
        "T(" + hs.name() + ")",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      hs/Cp_ + dimensionedScalar{"Tstd", dimTemperature, 298.15},
      zeroGradientFvPatchScalarField::typeName
    }
  };
  tT().min(Tmax_);
  tT().max(Tmin_);
  return tT;
}


inline const volScalarField& thermoSingleLayer::hsSp() const
{
  return hsSp_;
}


inline const volScalarField& thermoSingleLayer::hsSpPrimary() const
{
  return hsSpPrimary_;
}


inline const volScalarField& thermoSingleLayer::TPrimary() const
{
  return TPrimary_;
}


inline const PtrList<volScalarField>& thermoSingleLayer::YPrimary() const
{
  return YPrimary_;
}


inline const heatTransferModel& thermoSingleLayer::htcs() const
{
  return htcs_();
}


inline const heatTransferModel& thermoSingleLayer::htcw() const
{
  return htcw_();
}


inline const phaseChangeModel& thermoSingleLayer::phaseChange() const
{
  return phaseChange_();
}


inline const filmRadiationModel& thermoSingleLayer::radiation() const
{
  return radiation_();
}


inline tmp<scalarField> thermoSingleLayer::Qconvw(const label patchI) const
{
  const scalarField htc(htcw_->h()().boundaryField()[patchI]);
  const scalarField& Tp = T_.boundaryField()[patchI];
  const scalarField& Twp = Tw_.boundaryField()[patchI];
  return htc*(Tp - Twp);
}


inline tmp<scalarField> thermoSingleLayer::Qconvp(const label patchI) const
{
  const scalarField htc(htcs_->h()().boundaryField()[patchI]);
  const scalarField& Tp = T_.boundaryField()[patchI];
  const scalarField& Tpp = TPrimary_.boundaryField()[patchI];
  return htc*(Tp - Tpp);
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
