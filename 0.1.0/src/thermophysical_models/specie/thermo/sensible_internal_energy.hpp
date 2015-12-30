// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sensibleInternalEnergy
// Description
//   Thermodynamics mapping class to expose the sensible internal energy function
//   as the standard internal energy function e(T).
#ifndef sensible_internal_energy_hpp_
#define sensible_internal_energy_hpp_
namespace mousse
{
template<class Thermo>
class sensibleInternalEnergy
{
public:
  // Constructors
    //- Construct
    sensibleInternalEnergy()
    {}
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "sensibleInternalEnergy";
    }
    // Fundamental properties
      static word name()
      {
        return "e";
      }
      //- Sensible Internal energy [J/kmol]
      scalar he
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T) const
      {
        return thermo.es(p, T);
      }
      //- Heat capacity at constant volume [J/(kmol K)]
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
      //- Sensible internal energy [J/kg]
      scalar HE
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.Es(p, T);
      }
      //- Temperature from sensible internal energy
      //  given an initial temperature T0
      scalar THE
      (
        const Thermo& thermo,
        const scalar e,
        const scalar p,
        const scalar T0
      ) const
      {
        return thermo.TEs(e, p, T0);
      }
};
}  // namespace mousse
#endif
