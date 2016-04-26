#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_I_C3_H8_O_I_C3_H8_O_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_I_C3_H8_O_I_C3_H8_O_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::iC3H8O
// Description
//   iso-propanol

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


namespace mousse {

class iC3H8O
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
    NSRDSfunc0 sigma_;
    NSRDSfunc1 D_;
public:
  //- Runtime type information
  TYPE_NAME("iC3H8O");
  // Constructors
    //- Construct null
    iC3H8O();
    //- Constrcut from components
    iC3H8O
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
      const NSRDSfunc0& surfaceTension,
      const NSRDSfunc1& vapourDiffussivity
    );
    //- Construct from Istream
    iC3H8O(Istream& is);
    //- Construct from dictionary
    iC3H8O(const dictionary& dict);
    //- Construct copy
    iC3H8O(const iC3H8O& liq);
    //- Construct and return clone
    virtual autoPtr<liquidProperties> clone() const
    {
      return autoPtr<liquidProperties>{new iC3H8O{*this}};
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
    //- Liquid thermal conductivity  [W/(m K)]
    inline scalar K(scalar p, scalar T) const;
    //- Vapour thermal conductivity  [W/(m K)]
    inline scalar Kg(scalar p, scalar T) const;
    //- Surface tension [N/m]
    inline scalar sigma(scalar p, scalar T) const;
    //- Vapour diffussivity [m2/s]
    inline scalar D(scalar p, scalar T) const;
    //- Vapour diffussivity [m2/s] with specified binary pair
    // Note: behaves the same as D(p, T)
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
    friend Ostream& operator<<(Ostream& os, const iC3H8O& l)
    {
      l.writeData(os);
      return os;
    }
};

}  // namespace mousse


inline mousse::scalar mousse::iC3H8O::rho(scalar p, scalar T) const
{
  return rho_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::pv(scalar p, scalar T) const
{
  return pv_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::hl(scalar p, scalar T) const
{
  return hl_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::Cp(scalar p, scalar T) const
{
  return Cp_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::h(scalar p, scalar T) const
{
  return h_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::Cpg(scalar p, scalar T) const
{
  return Cpg_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::B(scalar p, scalar T) const
{
  return B_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::mu(scalar p, scalar T) const
{
  return mu_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::mug(scalar p, scalar T) const
{
  return mug_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::K(scalar p, scalar T) const
{
  return K_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::Kg(scalar p, scalar T) const
{
  return Kg_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::sigma(scalar p, scalar T) const
{
  return sigma_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::D(scalar p, scalar T) const
{
  return D_.f(p, T);
}


inline mousse::scalar mousse::iC3H8O::D(scalar p, scalar T, scalar) const
{
  // Use 'standard' NSRDS D(p, T) function
  return D(p, T);
}

#endif

