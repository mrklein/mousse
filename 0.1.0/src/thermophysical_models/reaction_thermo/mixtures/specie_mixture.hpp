#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_SPECIE_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_SPECIE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SpecieMixture
// Description
//   mousse::SpecieMixture

#include "scalar.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Forward declaration of classes
class dictionary;
class fvMesh;


template<class MixtureType>
class SpecieMixture
:
  public MixtureType
{
public:
  // Constructors
    //- Construct from dictionary, mesh and phase name
    SpecieMixture(const dictionary&, const fvMesh&, const word& phaseName);
  //- Destructor
  virtual ~SpecieMixture()
  {}
  // Member functions
    // Per specie properties
      //- Number of moles of the given specie []
      virtual scalar nMoles(const label speciei) const;
      //- Molecular weight of the given specie [kg/kmol]
      virtual scalar W(const label speciei) const;
    // Per specie thermo properties
      //- Heat capacity at constant pressure [J/(kg K)]
      virtual scalar Cp
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Heat capacity at constant volume [J/(kg K)]
      virtual scalar Cv
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Absolute enthalpy [J/kg]
      virtual scalar Ha
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Sensible enthalpy [J/kg]
      virtual scalar Hs
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Chemical enthalpy [J/kg]
      virtual scalar Hc(const label speciei) const;
      //- Entropy [J/(kg K)]
      virtual scalar S
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Sensible internal energy [J/kg]
      virtual scalar Es
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Gibbs free energy [J/kg]
      virtual scalar G
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Helmholtz free energy [J/kg]
      virtual scalar A
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
    // Per specie transport properties
      //- Dynamic viscosity [kg/m/s]
      virtual scalar mu
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Thermal conductivity [W/m/K]
      virtual scalar kappa
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Thermal diffusivity of enthalpy [kg/m/s]
      virtual scalar alphah
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
      //- Density [kg/m3]
      virtual scalar rho
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const;
    // Field properties abstract functions
    // provided here to avoid issues with inheritance of virtual functions
    // in heThermo
      //- Heat capacity at constant pressure for patch [J/kg/K]
      virtual tmp<scalarField> Cp
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Heat capacity at constant pressure for patch [J/kg/K]
      virtual tmp<volScalarField> Cp() const = 0;
      //- Heat capacity at constant volume for patch [J/kg/K]
      virtual tmp<scalarField> Cv
      (
        const scalarField& p,
        const scalarField& T,
        const label patchi
      ) const = 0;
      //- Heat capacity at constant volume [J/kg/K]
      virtual tmp<volScalarField> Cv() const = 0;
      //- Thermal diffusivity for temperature
      //  of mixture for patch [J/m/s/K]
      virtual tmp<scalarField> kappa
      (
        const label patchi
      ) const = 0;
      //- Thermal diffusivity for temperature of mixture [J/m/s/K]
      virtual tmp<volScalarField> kappa() const = 0;
};

}  // namespace mousse

#include "specie_mixture.ipp"

#endif
