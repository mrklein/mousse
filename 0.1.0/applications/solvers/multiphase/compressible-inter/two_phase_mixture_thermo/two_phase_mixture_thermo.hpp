#ifndef TWO_PHASE_MIXTURE_THERMO_HPP_
#define TWO_PHASE_MIXTURE_THERMO_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::twoPhaseMixtureThermo

#include "rho_thermo.hpp"
#include "psi_thermo.hpp"
#include "two_phase_mixture.hpp"


namespace mousse {

class twoPhaseMixtureThermo
:
  public psiThermo,
  public twoPhaseMixture
{
  // Private data
    //- Thermo-package of phase 1
    autoPtr<rhoThermo> thermo1_;
    //- Thermo-package of phase 2
    autoPtr<rhoThermo> thermo2_;
public:
  //- Runtime type information
  TYPE_NAME("twoPhaseMixtureThermo");
  // Constructors
    //- Construct from mesh
    twoPhaseMixtureThermo(const fvMesh& mesh);
  //- Destructor
  virtual ~twoPhaseMixtureThermo();
  // Member Functions
    const rhoThermo& thermo1() const { return thermo1_(); }
    const rhoThermo& thermo2() const { return thermo2_(); }
    rhoThermo& thermo1() { return thermo1_(); }
    rhoThermo& thermo2() { return thermo2_(); }
    //- Update properties
    virtual void correct();
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
        NOT_IMPLEMENTED("twoPhaseMixtureThermo::he()");
        return thermo1_->he();
      }
      //- Enthalpy/Internal energy [J/kg]
      virtual const volScalarField& he() const
      {
        NOT_IMPLEMENTED("twoPhaseMixtureThermo::he() const");
        return thermo1_->he();
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
};

}  // namespace mousse

#endif

