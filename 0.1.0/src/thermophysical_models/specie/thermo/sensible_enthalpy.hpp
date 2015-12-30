// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::sensibleEnthalpy
// Description
//   Thermodynamics mapping class to expose the sensible enthalpy function
//   as the standard enthalpy function h(T).
#ifndef sensible_enthalpy_hpp_
#define sensible_enthalpy_hpp_
namespace mousse
{
template<class Thermo>
class sensibleEnthalpy
{
public:
  // Constructors
    //- Construct
    sensibleEnthalpy()
    {}
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "sensibleEnthalpy";
    }
    // Fundamental properties
      static word name()
      {
        return "h";
      }
      // Sensible enthalpy [J/kmol]
      scalar he
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.hs(p, T);
      }
      // Heat capacity at constant pressure [J/(kmol K)]
      scalar cpv
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.cp(p, T);
      }
      //- cp/cp []
      scalar cpBycpv
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return 1;
      }
      // Sensible enthalpy [J/kg]
      scalar HE
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.Hs(p, T);
      }
      //- Temperature from sensible enthalpy
      //  given an initial temperature T0
      scalar THE
      (
        const Thermo& thermo,
        const scalar h,
        const scalar p,
        const scalar T0
      ) const
      {
        return thermo.THs(h, p, T0);
      }
};
}  // namespace mousse
#endif
