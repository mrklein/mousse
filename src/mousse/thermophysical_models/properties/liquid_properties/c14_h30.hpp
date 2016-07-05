#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_C14_H30_C14_H30_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_C14_H30_C14_H30_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::C14H30
// Description
//   nTetraDecane

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


namespace mousse {

class C14H30
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
  TYPE_NAME("C14H30");
  // Constructors
    //- Construct null
    C14H30();
    //- Construct from components
    C14H30
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
    C14H30(Istream& is);
    //- Construct from dictionary
    C14H30(const dictionary& dict);
    //- Construct copy
    C14H30(const C14H30& liq);
    //- Construct and return clone
    virtual autoPtr<liquidProperties> clone() const
    {
      return autoPtr<liquidProperties>{new C14H30{*this}};
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
    friend Ostream& operator<<(Ostream& os, const C14H30& l)
    {
      l.writeData(os);
      return os;
    }
};

}  // namespace mousse


inline mousse::scalar mousse::C14H30::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}


inline mousse::scalar mousse::C14H30::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}


inline mousse::scalar mousse::C14H30::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}


inline mousse::scalar mousse::C14H30::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}
inline mousse::scalar mousse::C14H30::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}


inline mousse::scalar mousse::C14H30::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}


inline mousse::scalar mousse::C14H30::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}


inline mousse::scalar mousse::C14H30::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}


inline mousse::scalar mousse::C14H30::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}


inline mousse::scalar mousse::C14H30::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}


inline mousse::scalar mousse::C14H30::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}


inline mousse::scalar mousse::C14H30::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}


inline mousse::scalar mousse::C14H30::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}


inline mousse::scalar mousse::C14H30::D(scalar p, scalar T, scalar Wb) const
{
  return D_.f(p, T, Wb);
}

#endif
