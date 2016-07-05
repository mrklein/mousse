#ifndef THERMOPHYSICAL_MODELS_SPECIE_THERMO_ABSOLUTE_INTERNAL_ENERGY_HPP_
#define THERMOPHYSICAL_MODELS_SPECIE_THERMO_ABSOLUTE_INTERNAL_ENERGY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::absoluteInternalEnergy
// Description
//   Thermodynamics mapping class to expose the absolute internal energy function
//   as the standard internal energy function e(T).

namespace mousse {

template<class Thermo>
class absoluteInternalEnergy
{
public:
  // Constructors
    //- Construct
    absoluteInternalEnergy()
    {}
  // Member Functions
    //- Return the instantiated type name
    static word typeName() { return "absoluteInternalEnergy"; }
    // Fundamental properties
      static word name() { return "ea"; }
      // Absolute internal energy [J/kmol]
      scalar he
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.ea(p, T);
      }
      // Heat capacity at constant volume [J/(kmol K)]
      scalar cpv
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.cv(p, T);
      }
      //- cp/cv []
      scalar cpBycpv
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.gamma(p, T);
      }
      // Absolute internal energy [J/kg]
      scalar HE
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.Ea(p, T);
      }
      //- Temperature from absolute internal energy
      //  given an initial temperature T0
      scalar THE
      (
        const Thermo& thermo,
        const scalar e,
        const scalar p,
        const scalar T0
      ) const
      {
        return thermo.TEa(e, p, T0);
      }
};

}  // namespace mousse

#endif

