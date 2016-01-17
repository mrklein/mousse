// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::bC10H7CH3
// Description
//   betaMethylNaphthalene
// SourceFiles
//   b_c10_h7_ch3.cpp
#ifndef b_c10_h7_ch3_hpp_
#define b_c10_h7_ch3_hpp_
#include "liquid_properties.hpp"
#include "nsrds_func0.hpp"
#include "nsrds_func1.hpp"
#include "nsrds_func2.hpp"
#include "nsrds_func3.hpp"
#include "nsrds_func4.hpp"
#include "nsrds_func5.hpp"
#include "nsrds_func6.hpp"
#include "nsrds_func7.hpp"
#include "nsrds_func14.hpp"
#include "api_diff_coef_func.hpp"
namespace mousse
{
class bC10H7CH3
:
  public liquidProperties
{
  // Private data
    NSRDSfunc5 rho_;
    NSRDSfunc1 pv_;
    NSRDSfunc6 hl_;
    NSRDSfunc0 Cp_;
    NSRDSfunc0 h_;
    NSRDSfunc7 Cpg_;
    NSRDSfunc4 B_;
    NSRDSfunc1 mu_;
    NSRDSfunc2 mug_;
    NSRDSfunc0 K_;
    NSRDSfunc2 Kg_;
    NSRDSfunc6 sigma_;
    APIdiffCoefFunc D_;
public:
  //- Runtime type information
  TYPE_NAME("bC11H10");
  // Constructors
    //- Construct null
    bC10H7CH3();
    //- Construct from components
    bC10H7CH3
    (
      const liquidProperties& l,
      const NSRDSfunc5& density,
      const NSRDSfunc1& vapourPressure,
      const NSRDSfunc6& heatOfVapourisation,
      const NSRDSfunc0& heatCapacity,
      const NSRDSfunc0& enthalpy,
      const NSRDSfunc7& idealGasHeatCapacity,
      const NSRDSfunc4& secondVirialCoeff,
      const NSRDSfunc1& dynamicViscosity,
      const NSRDSfunc2& vapourDynamicViscosity,
      const NSRDSfunc0& thermalConductivity,
      const NSRDSfunc2& vapourThermalConductivity,
      const NSRDSfunc6& surfaceTension,
      const APIdiffCoefFunc& vapourDiffussivity
    );
    //- Construct from Istream
    bC10H7CH3(Istream& is);
    //- Construct from dictionary
    bC10H7CH3(const dictionary& dict);
    //- Construct copy
    bC10H7CH3(const bC10H7CH3& liq);
    //- Construct and return clone
    virtual autoPtr<liquidProperties> clone() const
    {
      return autoPtr<liquidProperties>(new bC10H7CH3(*this));
    }
  // Member Functions
    //- Liquid density [kg/m^3]
    inline scalar rho(scalar p, scalar T) const;
    //- Vapour pressure [Pa]
    inline scalar pv(scalar p, scalar T) const;
    //- Heat of vapourisation [J/kg]
    inline scalar hl(scalar p, scalar T) const;
    //- Liquid heat capacity [J/(kg K)]
    inline scalar Cp(scalar p, scalar T) const;
    //- Liquid Enthalpy [J/(kg)]
    inline scalar h(scalar p, scalar T) const;
    //- Ideal gas heat capacity [J/(kg K)]
    inline scalar Cpg(scalar p, scalar T) const;
    //- Second Virial Coefficient [m^3/kg]
    inline scalar B(scalar p, scalar T) const;
    //- Liquid viscosity [Pa s]
    inline scalar mu(scalar p, scalar T) const;
    //- Vapour viscosity [Pa s]
    inline scalar mug(scalar p, scalar T) const;
    //- Liquid thermal conductivity [W/(m K)]
    inline scalar K(scalar p, scalar T) const;
    //- Vapour thermal conductivity [W/(m K)]
    inline scalar Kg(scalar p, scalar T) const;
    //- Surface tension [N/m]
    inline scalar sigma(scalar p, scalar T) const;
    //- Vapour diffussivity [m2/s]
    inline scalar D(scalar p, scalar T) const;
    //- Vapour diffussivity [m2/s] with specified binary pair
    inline scalar D(scalar p, scalar T, scalar Wb) const;
  // I-O
    //- Write the function coefficients
    void writeData(Ostream& os) const
    {
      liquidProperties::writeData(os); os << nl;
      rho_.writeData(os); os << nl;
      pv_.writeData(os); os << nl;
      hl_.writeData(os); os << nl;
      Cp_.writeData(os); os << nl;
      h_.writeData(os); os << nl;
      Cpg_.writeData(os); os << nl;
      B_.writeData(os); os << nl;
      mu_.writeData(os); os << nl;
      mug_.writeData(os); os << nl;
      K_.writeData(os); os << nl;
      Kg_.writeData(os); os << nl;
      sigma_.writeData(os); os << nl;
      D_.writeData(os); os << endl;
    }
    //- Ostream Operator
    friend Ostream& operator<<(Ostream& os, const bC10H7CH3& l)
    {
      l.writeData(os);
      return os;
    }
};
}  // namespace mousse

inline mousse::scalar mousse::bC10H7CH3::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}
inline mousse::scalar mousse::bC10H7CH3::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}
#endif
