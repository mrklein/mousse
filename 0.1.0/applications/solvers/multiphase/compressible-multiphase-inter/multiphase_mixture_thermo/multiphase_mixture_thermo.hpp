#ifndef MULTIPHASE_MIXTURE_THERMO_HPP_
#define MULTIPHASE_MIXTURE_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiphaseMixtureThermo

#include "phase_model.hpp"
#include "ptr_dictionary.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "rho_thermo.hpp"
#include "psi_thermo.hpp"


namespace mousse {

class multiphaseMixtureThermo
:
  public psiThermo
{
public:
  class interfacePair
  :
    public Pair<word>
  {
  public:
    class hash
    :
      public Hash<interfacePair>
    {
    public:
      hash()
      {}
      label operator()(const interfacePair& key) const
      {
        return word::hash()(key.first()) + word::hash()(key.second());
      }
    };
    // Constructors
      interfacePair()
      {}
      interfacePair(const word& alpha1Name, const word& alpha2Name)
      :
        Pair<word>{alpha1Name, alpha2Name}
      {}
      interfacePair(const phaseModel& alpha1, const phaseModel& alpha2)
      :
        Pair<word>{alpha1.name(), alpha2.name()}
      {}
    // Friend Operators
      friend bool operator==
      (
        const interfacePair& a,
        const interfacePair& b
      )
      {
        return
        (
          ((a.first() == b.first()) && (a.second() == b.second()))
          || ((a.first() == b.second()) && (a.second() == b.first()))
        );
      }
      friend bool operator!=
      (
        const interfacePair& a,
        const interfacePair& b
      )
      {
        return (!(a == b));
      }
  };
private:
  // Private data
    //- Dictionary of phases
    PtrDictionary<phaseModel> phases_;
    const fvMesh& mesh_;
    const volVectorField& U_;
    const surfaceScalarField& phi_;
    surfaceScalarField rhoPhi_;
    volScalarField alphas_;
    typedef HashTable<scalar, interfacePair, interfacePair::hash> sigmaTable;
    sigmaTable sigmas_;
    dimensionSet dimSigma_;
    //- Stabilisation for normalisation of the interface normal
    const dimensionedScalar deltaN_;
    //- Conversion factor for degrees into radians
    static const scalar convertToRad;
  // Private member functions
    void calcAlphas();
    void solveAlphas(const scalar cAlpha);
    tmp<surfaceVectorField> nHatfv
    (
      const volScalarField& alpha1,
      const volScalarField& alpha2
    ) const;
    tmp<surfaceScalarField> nHatf
    (
      const volScalarField& alpha1,
      const volScalarField& alpha2
    ) const;
    void correctContactAngle
    (
      const phaseModel& alpha1,
      const phaseModel& alpha2,
      surfaceVectorField::GeometricBoundaryField& nHatb
    ) const;
    tmp<volScalarField> K
    (
      const phaseModel& alpha1,
      const phaseModel& alpha2
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("multiphaseMixtureThermo");
  // Constructors
    //- Construct from components
    multiphaseMixtureThermo
    (
      const volVectorField& U,
      const surfaceScalarField& phi
    );
  //- Destructor
  virtual ~multiphaseMixtureThermo()
  {}
  // Member Functions
    //- Return the phases
    const PtrDictionary<phaseModel>& phases() const { return phases_; }
    //- Return non-const access to the phases
    PtrDictionary<phaseModel>& phases() { return phases_; }
    //- Return the velocity
    const volVectorField& U() const { return U_; }
    //- Return the volumetric flux
    const surfaceScalarField& phi() const { return phi_; }
    const surfaceScalarField& rhoPhi() const { return rhoPhi_; }
    //- Update properties
    virtual void correct();
    //- Update densities for given pressure change
    void correctRho(const volScalarField& dp);
    //- Return true if the equation of state is incompressible
    //  i.e. rho != f(p)
    virtual bool incompressible() const;
    //- Return true if the equation of state is isochoric
    //  i.e. rho = const
    virtual bool isochoric() const;
    // Access to thermodynamic state variables
      //- Enthalpy/Internal energy [J/kg]
      //  Non-const access allowed for transport equations
      virtual volScalarField& he()
      {
        NOT_IMPLEMENTED("multiphaseMixtureThermo::he()");
        return phases_[0].thermo().he();
      }
      //- Enthalpy/Internal energy [J/kg]
      virtual const volScalarField& he() const
      {
        NOT_IMPLEMENTED("multiphaseMixtureThermo::he() const");
        return phases_[0].thermo().he();
      }
      //- Enthalpy/Internal energy
      //  for given pressure and temperature [J/kg]
      virtual tmp<volScalarField> he
      (
        const volScalarField& p,
        const volScalarField& T
      ) const;
      //- Enthalpy/Internal energy for cell-set [J/kg]
      virtual tmp<scalarField> he
      (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
      ) const;
      //- Enthalpy/Internal energy for patch [J/kg]
      virtual tmp<scalarField> he
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Chemical enthalpy [J/kg]
      virtual tmp<volScalarField> hc() const;
      //- Temperature from enthalpy/internal energy for cell-set
      virtual tmp<scalarField> THE
      (
        const scalarField& h,
        const scalarField& p,
        const scalarField& T0,      // starting temperature
        const labelList& cells
      ) const;
      //- Temperature from enthalpy/internal energy for patch
      virtual tmp<scalarField> THE
      (
        const scalarField& h,
        const scalarField& p,
        const scalarField& T0,      // starting temperature
        const label patchi
      ) const;
    // Fields derived from thermodynamic state variables
      //- Density [kg/m^3]
      virtual tmp<volScalarField> rho() const;
      //- Density for patch [kg/m^3]
      virtual tmp<scalarField> rho(const label patchi) const;
      //- Heat capacity at constant pressure [J/kg/K]
      virtual tmp<volScalarField> Cp() const;
      //- Heat capacity at constant pressure for patch [J/kg/K]
      virtual tmp<scalarField> Cp
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Heat capacity at constant volume [J/kg/K]
      virtual tmp<volScalarField> Cv() const;
      //- Heat capacity at constant volume for patch [J/kg/K]
      virtual tmp<scalarField> Cv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Gamma = Cp/Cv []
      virtual tmp<volScalarField> gamma() const;
      //- Gamma = Cp/Cv for patch []
      virtual tmp<scalarField> gamma
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Heat capacity at constant pressure/volume [J/kg/K]
      virtual tmp<volScalarField> Cpv() const;
      //- Heat capacity at constant pressure/volume for patch [J/kg/K]
      virtual tmp<scalarField> Cpv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
      //- Heat capacity ratio []
      virtual tmp<volScalarField> CpByCpv() const;
      //- Heat capacity ratio for patch []
      virtual tmp<scalarField> CpByCpv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const;
    // Fields derived from transport state variables
      //- Kinematic viscosity of mixture [m^2/s]
      virtual tmp<volScalarField> nu() const;
      //- Kinematic viscosity of mixture for patch [m^2/s]
      virtual tmp<scalarField> nu(const label patchi) const;
      //- Thermal diffusivity for temperature of mixture [J/m/s/K]
      virtual tmp<volScalarField> kappa() const;
      //- Thermal diffusivity of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> kappa
      (
        const label patchi
      ) const;
      //- Effective thermal diffusivity of mixture [J/m/s/K]
      virtual tmp<volScalarField> kappaEff
      (
        const volScalarField& alphat
      ) const;
      //- Effective thermal diffusivity of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> kappaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const;
      //- Effective thermal diffusivity of mixture [J/m/s/K]
      virtual tmp<volScalarField> alphaEff
      (
        const volScalarField& alphat
      ) const;
      //- Effective thermal diffusivity of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> alphaEff
      (
        const scalarField& alphat,
        const label patchi
      ) const;
    //- Return the phase-averaged reciprocal Cv
    tmp<volScalarField> rCv() const;
    tmp<surfaceScalarField> surfaceTensionForce() const;
    //- Indicator of the proximity of the interface
    //  Field values are 1 near and 0 away for the interface.
    tmp<volScalarField> nearInterface() const;
    //- Solve for the mixture phase-fractions
    void solve();
};

}  // namespace mousse

#endif

