#ifndef THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_BASIC_SPECIE_MIXTURE_HPP_
#define THERMOPHYSICAL_MODELS_REACTION_THERMO_MIXTURES_BASIC_SPECIE_MIXTURE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::basicSpecieMixture
// Description
//   Specialization of basicMultiComponentMixture for a mixture consisting
//   of a number for molecular species.

#include "basic_multi_component_mixture.hpp"


namespace mousse {

class basicSpecieMixture
:
  public basicMultiComponentMixture
{
public:
  //- Run time type information
  TYPE_NAME("basicSpecieMixture");
  //- The base class of the mixture
  typedef basicSpecieMixture basicMixtureType;
  // Constructors
    //- Construct from dictionary, species names, mesh and phase name
    basicSpecieMixture
    (
      const dictionary&,
      const wordList& specieNames,
      const fvMesh&,
      const word&
    );
  //- Destructor
  virtual ~basicSpecieMixture()
  {}
  // Member functions
    // Per specie properties
      //- Number of moles of the given specie []
      virtual scalar nMoles(const label speciei) const = 0;
      //- Molecular weight of the given specie [kg/kmol]
      virtual scalar W(const label speciei) const = 0;
      //- Molecular weight of the mixture [kg/kmol]
      tmp<volScalarField> W() const;
    // Per specie thermo properties
      //- Heat capacity at constant pressure [J/(kg K)]
      virtual scalar Cp
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Heat capacity at constant volume [J/(kg K)]
      virtual scalar Cv
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Absolute enthalpy [J/kg]
      virtual scalar Ha
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Sensible enthalpy [J/kg]
      virtual scalar Hs
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Chemical enthalpy [J/kg]
      virtual scalar Hc(const label speciei) const = 0;
      //- Entropy [J/(kg K)]
      virtual scalar S
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Sensible internal energy [J/kg]
      virtual scalar Es
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Gibbs free energy [J/kg]
      virtual scalar G
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Helmholtz free energy [J/kg]
      virtual scalar A
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
    // Per specie transport properties
      //- Dynamic viscosity [kg/m/s]
      virtual scalar mu
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Thermal conductivity [W/m/K]
      virtual scalar kappa
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Thermal diffusivity of enthalpy [kg/m/s]
      virtual scalar alphah
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
      //- Density [kg/m3]
      virtual scalar rho
      (
        const label speciei,
        const scalar p,
        const scalar T
      ) const = 0;
};

}  // namespace mousse

#endif

