#ifndef REGION_MODELS_SURFACE_FILM_MODELS_KINEMATIC_SINGLE_LAYER_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_KINEMATIC_SINGLE_LAYER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kinematicSingleLayer
// Description
//   Kinematic form of single-cell layer surface film model

#include "surface_film_model.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrices.hpp"
#include "injection_model_list.hpp"
#include "force_list.hpp"
#include "film_turbulence_model.hpp"
#include "film_thermo_model.hpp"
#include "surface_interpolate.hpp"
#include "fvc_surface_integrate.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

// Forward class declarations
class filmThermoModel;


class kinematicSingleLayer
:
  public surfaceFilmModel
{
protected:
  // Protected data
    // Solution parameters
      //- Momentum predictor
      Switch momentumPredictor_;
      //- Number of outer correctors
      label nOuterCorr_;
      //- Number of PISO-like correctors
      label nCorr_;
      //- Number of non-orthogonal correctors
      label nNonOrthCorr_;
      //- Cumulative continuity error
      scalar cumulativeContErr_;
      //- Small delta
      const dimensionedScalar deltaSmall_;
      //- Film thickness above which Courant number calculation in valid
      scalar deltaCoLimit_;
    // Thermo properties
      // Fields
        //- Density / [kg/m3]
        volScalarField rho_;
        //- Dynamic viscosity / [Pa.s]
        volScalarField mu_;
        //- Surface tension / [m/s2]
        volScalarField sigma_;
    // Fields
      //- Film thickness / [m]
      volScalarField delta_;
      //- Film coverage indicator, 1 = covered, 0 = uncovered / []
      volScalarField alpha_;
      //- Velocity - mean / [m/s]
      volVectorField U_;
      //- Velocity - surface / [m/s]
      volVectorField Us_;
      //- Velocity - wall / [m/s]
      volVectorField Uw_;
      //- Film thickness*density (helper field) / [kg/m2]
      volScalarField deltaRho_;
      //- Mass flux (includes film thickness) / [kg.m/s]
      surfaceScalarField phi_;
      // Transfer fields
        //- Film mass available for transfer to the primary region
        volScalarField primaryMassTrans_;
        //- Film mass available for transfer to cloud
        volScalarField cloudMassTrans_;
        //- Parcel diameters originating from film to cloud
        volScalarField cloudDiameterTrans_;
    // Source term fields
      // Film region - registered to the film region mesh
      // Note: need boundary value mapped from primary region, and then
      // pushed into the patch internal field
        //- Momementum / [kg/m/s2]
        volVectorField USp_;
        //- Pressure / [Pa]
        volScalarField pSp_;
        //- Mass / [kg/m2/s]
        volScalarField rhoSp_;
      // Primary region - registered to the primary region mesh
      // Internal use only - not read-in
        //- Momementum / [kg/m/s2]
        volVectorField USpPrimary_;
        //- Pressure / [Pa]
        volScalarField pSpPrimary_;
        //- Mass / [kg/m2/s]
        volScalarField rhoSpPrimary_;
    // Fields mapped from primary region - registered to the film region
    // Note: need both boundary AND patch internal fields to be mapped
      //- Velocity / [m/s]
      volVectorField UPrimary_;
      //- Pressure / [Pa]
      volScalarField pPrimary_;
      //- Density / [kg/m3]
      volScalarField rhoPrimary_;
      //- Viscosity / [Pa.s]
      volScalarField muPrimary_;
    // Sub-models
      //- Film thermo model
      autoPtr<filmThermoModel> filmThermo_;
      //- Available mass for transfer via sub-models
      scalarField availableMass_;
      //- Cloud injection
      injectionModelList injection_;
      //- Turbulence model
      autoPtr<filmTurbulenceModel> turbulence_;
      //- List of film forces
      forceList forces_;
   // Checks
     //- Cumulative mass added via sources [kg]
     scalar addedMassTotal_;
  // Protected member functions
    //- Read control parameters from dictionary
    virtual bool read();
    //- Correct the thermo fields
    virtual void correctThermoFields();
    //- Reset source term fields
    virtual void resetPrimaryRegionSourceTerms();
    //- Transfer thermo fields from the primary region to the film region
    virtual void transferPrimaryRegionThermoFields();
    //- Transfer source fields from the primary region to the film region
    virtual void transferPrimaryRegionSourceFields();
    //- Explicit pressure source contribution
    virtual tmp<volScalarField> pu();
    //- Implicit pressure source coefficient
    virtual tmp<volScalarField> pp();
    //- Correct film coverage field
    virtual void correctAlpha();
    //- Update the film sub-models
    virtual void updateSubmodels();
    //- Continuity check
    virtual void continuityCheck();
    //- Update film surface velocities
    virtual void updateSurfaceVelocities();
    //- Constrain a film region master/slave boundaries of a field to a
    //  given value
    template<class Type>
    void constrainFilmField
    (
      Type& field,
      const typename Type::cmptType& value
    );
    // Equations
      //- Solve continuity equation
      virtual void solveContinuity();
      //- Solve for film velocity
      virtual tmp<fvVectorMatrix> solveMomentum
      (
        const volScalarField& pu,
        const volScalarField& pp
      );
      //- Solve coupled velocity-thickness equations
      virtual void solveThickness
      (
        const volScalarField& pu,
        const volScalarField& pp,
        const fvVectorMatrix& UEqn
      );
public:
  //- Runtime type information
  TYPE_NAME("kinematicSingleLayer");
  // Constructors
    //- Construct from components
    kinematicSingleLayer
    (
      const word& modelType,
      const fvMesh& mesh,
      const dimensionedVector& g,
      const word& regionType,
      const bool readFields = true
    );
    //- Disallow default bitwise copy construct
    kinematicSingleLayer(const kinematicSingleLayer&) = delete;
    //- Disallow default bitwise assignment
    kinematicSingleLayer& operator=(const kinematicSingleLayer&) = delete;
  //- Destructor
  virtual ~kinematicSingleLayer();
  // Member Functions
    // Solution parameters
      //- Courant number evaluation
      virtual scalar CourantNumber() const;
      //- Return the momentum predictor
      inline const Switch& momentumPredictor() const;
      //- Return the number of outer correctors
      inline label nOuterCorr() const;
      //- Return the number of PISO correctors
      inline label nCorr() const;
      //- Return the number of non-orthogonal correctors
      inline label nNonOrthCorr() const;
      //- Return small delta
      inline const dimensionedScalar& deltaSmall() const;
    // Thermo properties
      //- Return const access to the dynamic viscosity / [Pa.s]
      inline const volScalarField& mu() const;
      //- Return const access to the surface tension / [m/s2]
      inline const volScalarField& sigma() const;
    // Fields
      //- Return const access to the film thickness / [m]
      inline const volScalarField& delta() const;
      //- Return the film coverage, 1 = covered, 0 = uncovered / []
      inline const volScalarField& alpha() const;
      //- Return the film velocity [m/s]
      virtual const volVectorField& U() const;
      //- Return the film surface velocity [m/s]
      virtual const volVectorField& Us() const;
      //- Return the film wall velocity [m/s]
      virtual const volVectorField& Uw() const;
      //- Return the film thickness*density (helper field) [kg/m3]
      virtual const volScalarField& deltaRho() const;
      //- Return the film flux [kg.m/s]
      virtual const surfaceScalarField& phi() const;
      //- Return the film density [kg/m3]
      virtual const volScalarField& rho() const;
      //- Return the film mean temperature [K]
      virtual const volScalarField& T() const;
      //- Return the film surface temperature [K]
      virtual const volScalarField& Ts() const;
      //- Return the film wall temperature [K]
      virtual const volScalarField& Tw() const;
      //- Return the film specific heat capacity [J/kg/K]
      virtual const volScalarField& Cp() const;
      //- Return the film thermal conductivity [W/m/K]
      virtual const volScalarField& kappa() const;
      // Transfer fields - to the primary region
        //- Return mass transfer source - Eulerian phase only
        virtual tmp<volScalarField> primaryMassTrans() const;
        //- Return the film mass available for transfer to cloud
        virtual const volScalarField& cloudMassTrans() const;
        //- Return the parcel diameters originating from film to cloud
        virtual const volScalarField& cloudDiameterTrans() const;
    // External helper functions
      //- External hook to add sources to the film
      virtual void addSources
      (
        const label patchI,            // patchI on primary region
        const label faceI,             // faceI of patchI
        const scalar massSource,       // [kg]
        const vector& momentumSource,  // [kg.m/s] (tang'l momentum)
        const scalar pressureSource,   // [kg.m/s] (normal momentum)
        const scalar energySource = 0  // [J]
      );
    // Source fields (read/write access)
      // Primary region
        //- Momementum / [kg/m/s2]
        inline volVectorField& USpPrimary();
        //- Pressure / [Pa]
        inline volScalarField& pSpPrimary();
        //- Mass / [kg/m2/s]
        inline volScalarField& rhoSpPrimary();
      // Film region
        //- Momentum / [kg/m/s2]
        inline volVectorField& USp();
        //- Pressure / [Pa]
        inline volScalarField& pSp();
        //- Mass / [kg/m2/s]
        inline volScalarField& rhoSp();
        //- Momentum / [kg/m/s2]
        inline const volVectorField& USp() const;
        //- Pressure / [Pa]
        inline const volScalarField& pSp() const;
        //- Mass / [kg/m2/s]
        inline const volScalarField& rhoSp() const;
    // Fields mapped from primary region
      //- Velocity / [m/s]
      inline const volVectorField& UPrimary() const;
      //- Pressure / [Pa]
      inline const volScalarField& pPrimary() const;
      //- Density / [kg/m3]
      inline const volScalarField& rhoPrimary() const;
      //- Viscosity / [Pa.s]
      inline const volScalarField& muPrimary() const;
    // Sub-models
      //- Film thermo
      inline const filmThermoModel& filmThermo() const;
      //- Injection
      inline injectionModelList& injection();
      //- Turbulence
      inline const filmTurbulenceModel& turbulence() const;
    // Helper functions
      //- Return the current film mass
      inline tmp<volScalarField> mass() const;
      //- Return the net film mass available over the next integration
      inline tmp<volScalarField> netMass() const;
      //- Return the change in film mass due to sources/sinks
      inline tmp<volScalarField> deltaMass() const;
      //- Return the gravity normal-to-patch component contribution
      inline tmp<volScalarField> gNorm() const;
      //- Return the gravity normal-to-patch component contribution
      //  Clipped so that only non-zero if g & nHat_ < 0
      inline tmp<volScalarField> gNormClipped() const;
      //- Return the gravity tangential component contributions
      inline tmp<volVectorField> gTan() const;
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

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

inline const Switch& kinematicSingleLayer::momentumPredictor() const
{
  return momentumPredictor_;
}


inline label kinematicSingleLayer::nOuterCorr() const
{
  return nOuterCorr_;
}


inline label kinematicSingleLayer::nCorr() const
{
  return nCorr_;
}


inline label kinematicSingleLayer::nNonOrthCorr() const
{
  return nNonOrthCorr_;
}


inline const dimensionedScalar& kinematicSingleLayer::deltaSmall() const
{
  return deltaSmall_;
}


inline const volScalarField& kinematicSingleLayer::mu() const
{
  return mu_;
}


inline const volScalarField& kinematicSingleLayer::sigma() const
{
  return sigma_;
}


inline const volScalarField& kinematicSingleLayer::delta() const
{
  return delta_;
}


inline const volScalarField& kinematicSingleLayer::alpha() const
{
  return alpha_;
}


inline volVectorField& kinematicSingleLayer::USpPrimary()
{
  return USpPrimary_;
}


inline volScalarField& kinematicSingleLayer::pSpPrimary()
{
  return pSpPrimary_;
}


inline volScalarField& kinematicSingleLayer::rhoSpPrimary()
{
  return rhoSpPrimary_;
}


inline volVectorField& kinematicSingleLayer::USp()
{
  return USp_;
}


inline volScalarField& kinematicSingleLayer::pSp()
{
  return pSp_;
}


inline volScalarField& kinematicSingleLayer::rhoSp()
{
  return rhoSp_;
}


inline const volVectorField& kinematicSingleLayer::USp() const
{
  return USp_;
}


inline const volScalarField& kinematicSingleLayer::pSp() const
{
  return pSp_;
}


inline const volScalarField& kinematicSingleLayer::rhoSp() const
{
  return rhoSp_;
}


inline const volVectorField& kinematicSingleLayer::UPrimary() const
{
  return UPrimary_;
}


inline const volScalarField& kinematicSingleLayer::pPrimary() const
{
  return pPrimary_;
}


inline const volScalarField& kinematicSingleLayer::rhoPrimary() const
{
  return rhoPrimary_;
}


inline const volScalarField& kinematicSingleLayer::muPrimary() const
{
  return muPrimary_;
}


inline const filmThermoModel& kinematicSingleLayer::filmThermo() const
{
  return filmThermo_();
}


inline injectionModelList& kinematicSingleLayer::injection()
{
  return injection_;
}


inline const filmTurbulenceModel& kinematicSingleLayer::turbulence() const
{
  return turbulence_();
}


inline tmp<volScalarField> kinematicSingleLayer::mass() const
{
  return rho_*delta_*magSf();
}


inline tmp<volScalarField> kinematicSingleLayer::netMass() const
{
  return
    fvc::surfaceSum(pos(phi_)*phi_/(fvc::interpolate(delta_) + deltaSmall_))
   *time().deltaT()
   + rho_*delta_*magSf();
}


inline tmp<volScalarField> kinematicSingleLayer::deltaMass() const
{
  return rhoSp_*magSf()*time().deltaT();
}


inline tmp<volScalarField> kinematicSingleLayer::gNorm() const
{
  return
    tmp<volScalarField>
    {
      new volScalarField
      {
        // IOobject
        {
          "gNorm",
            time().timeName(),
            regionMesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        },
        g_ & nHat(),
        zeroGradientFvPatchScalarField::typeName
      }
    };
}


inline tmp<volScalarField> kinematicSingleLayer::gNormClipped() const
{
  tmp<volScalarField> tgNormClipped
  {
    new volScalarField
    {
      {
        "gNormClipped",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      g_ & nHat(),
      zeroGradientFvPatchScalarField::typeName
    }
  };
  volScalarField& gNormClipped = tgNormClipped();
  gNormClipped.min(0.0);
  return tgNormClipped;
}


inline tmp<volVectorField> kinematicSingleLayer::gTan() const
{
  tmp<volVectorField> tgtan
  {
    new volVectorField
    {
      // IOobject
      {
        "gTan",
        time().timeName(),
        regionMesh(),
        IOobject::NO_READ,
        IOobject::NO_WRITE
      },
      g_ - nHat()*gNorm(),
      zeroGradientFvPatchVectorField::typeName
    }
  };

  return tgtan;
}

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#include "kinematic_single_layer.ipp"

#endif
