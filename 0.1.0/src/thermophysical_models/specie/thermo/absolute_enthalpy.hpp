// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::absoluteEnthalpy
// Description
//   Thermodynamics mapping class to expose the absolute enthalpy function
//   as the standard enthalpy function h(T).
#ifndef absolute_enthalpy_hpp_
#define absolute_enthalpy_hpp_
namespace mousse
{
template<class Thermo>
class absoluteEnthalpy
{
public:
  // Constructors
    //- Construct
    absoluteEnthalpy()
    {}
  // Member Functions
    //- Return the instantiated type name
    static word typeName()
    {
      return "absoluteEnthalpy";
    }
    // Fundamental properties
      static word name()
      {
        return "ha";
      }
      // Absolute enthalpy [J/kmol]
      scalar he
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.ha(p, T);
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
      // Absolute enthalpy [J/kg]
      scalar HE
      (
        const Thermo& thermo,
        const scalar p,
        const scalar T
      ) const
      {
        return thermo.Ha(p, T);
      }
      //- Temperature from absolute enthalpy
      //  given an initial temperature T0
      scalar THE
      (
        const Thermo& thermo,
        const scalar h,
        const scalar p,
        const scalar T0
      ) const
      {
        return thermo.THa(h, p, T0);
      }
};
}  // namespace mousse
#endif
