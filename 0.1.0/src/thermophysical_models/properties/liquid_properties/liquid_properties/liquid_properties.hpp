#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_LIQUID_PROPERTIES_LIQUID_PROPERTIES_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_LIQUID_PROPERTIES_LIQUID_PROPERTIES_LIQUID_PROPERTIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liquidProperties
// Description
//   The thermophysical properties of a liquidProperties
// SourceFiles
//   liquid_properties.cpp
#include "scalar.hpp"
#include "iostreams.hpp"
#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"
namespace mousse
{
class liquidProperties
{
  // Private data
    //- Molecular weight [kg/kmol]
    scalar W_;
    //- Critical temperature [K]
    scalar Tc_;
    //- Critical pressure [Pa]
    scalar Pc_;
    //- Critical volume [m^3/kmol]
    scalar Vc_;
    //- Critical compressibility factor []
    scalar Zc_;
    //- Triple point temperature [K]
    scalar Tt_;
    //- Triple point pressure [Pa]
    scalar Pt_;
    //- Normal boiling temperature [K]
    scalar Tb_;
    //- Dipole moment []
    scalar dipm_;
    //- Pitzer's accentric factor []
    scalar omega_;
    //- Solubility parameter [(J/m^3)^0.5]
    scalar delta_;
public:
  TYPE_NAME("liquidProperties");
  // Declare run-time constructor selection tables
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      liquidProperties,
      ,
      (),
      ()
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      liquidProperties,
      Istream,
      (Istream& is),
      (is)
    );
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      liquidProperties,
      dictionary,
      (const dictionary& dict),
      (dict)
    );
  // Constructors
    //- Construct from components
    liquidProperties
    (
      scalar W,
      scalar Tc,
      scalar Pc,
      scalar Vc,
      scalar Zc,
      scalar Tt,
      scalar Pt,
      scalar Tb,
      scalar dipm,
      scalar omega,
      scalar delta
    );
    //- Construct from Istream
    liquidProperties(Istream& is);
    //- Construct from dictionary
    liquidProperties(const dictionary& dict);
    //- Construct copy
    liquidProperties(const liquidProperties& liq);
    //- Construct and return clone
    virtual autoPtr<liquidProperties> clone() const
    {
      return autoPtr<liquidProperties>(new liquidProperties(*this));
    }
  // Selectors
    //- Return a pointer to a new liquidProperties created from input
    static autoPtr<liquidProperties> New(Istream& is);
    //- Return a pointer to a new liquidProperties created from dictionary
    static autoPtr<liquidProperties> New(const dictionary& dict);
  //- Destructor
  virtual ~liquidProperties()
  {}
  // Member Functions
    // Physical constants which define the specie
      //- Molecular weight [kg/kmol]
      inline scalar W() const;
      //- Critical temperature [K]
      inline scalar Tc() const;
      //- Critical pressure [Pa]
      inline scalar Pc() const;
      //- Critical volume [m^3/kmol]
      inline scalar Vc() const;
      //- Critical compressibilty factor
      inline scalar Zc() const;
      //- Triple point temperature [K]
      inline scalar Tt() const;
      //- Triple point pressure [Pa]
      inline scalar Pt() const;
      //- Normal boiling temperature [K]
      inline scalar Tb() const;
      //- Dipole moment []
      inline scalar dipm() const;
      //- Pitzer's ascentric factor []
      inline scalar omega() const;
      //- Solubility parameter [(J/m^3)^(1/2)]
      inline scalar delta() const;
    // Physical property pure virtual functions
      //- Liquid rho [kg/m^3]
      virtual scalar rho(scalar p, scalar T) const;
      //- Vapour pressure [Pa]
      virtual scalar pv(scalar p, scalar T) const;
      //- Heat of vapourisation [J/kg]
      virtual scalar hl(scalar p, scalar T) const;
      //- Liquid heat capacity [J/(kg K)]
      virtual scalar Cp(scalar p, scalar T) const;
      //- Liquid enthalpy [J/kg] - reference to 298.15 K
      virtual scalar h(scalar p, scalar T) const;
      //- Ideal gas heat capacity [J/(kg K)]
      virtual scalar Cpg(scalar p, scalar T) const;
      //- Liquid viscosity [Pa s]
      virtual scalar mu(scalar p, scalar T) const;
      //- Vapour viscosity [Pa s]
      virtual scalar mug(scalar p, scalar T) const;
      //- Liquid thermal conductivity  [W/(m K)]
      virtual scalar K(scalar p, scalar T) const;
      //- Vapour thermal conductivity  [W/(m K)]
      virtual scalar Kg(scalar p, scalar T) const;
      //- Surface tension [N/m]
      virtual scalar sigma(scalar p, scalar T) const;
      //- Vapour diffussivity [m2/s]
      virtual scalar D(scalar p, scalar T) const;
      //- Vapour diffussivity [m2/s] with specified binary pair
      virtual scalar D(scalar p, scalar T, scalar Wb) const;
      //- Invert the vapour pressure relationship to retrieve the
      //  boiling temperuture as a function of pressure
      virtual scalar pvInvert(scalar p) const;
  // I-O
    //- Write the function coefficients
    virtual void writeData(Ostream& os) const;
    //- Ostream Operator
    friend Ostream& operator<<(Ostream& os, const liquidProperties& l)
    {
      l.writeData(os);
      return os;
    }
};
}  // namespace mousse

inline mousse::scalar mousse::liquidProperties::W() const
{
  return W_;
}
inline mousse::scalar mousse::liquidProperties::Tc() const
{
  return Tc_;
}
inline mousse::scalar mousse::liquidProperties::Pc() const
{
  return Pc_;
}
inline mousse::scalar mousse::liquidProperties::Vc() const
{
  return Vc_;
}
inline mousse::scalar mousse::liquidProperties::Zc() const
{
  return Zc_;
}
inline mousse::scalar mousse::liquidProperties::Tt() const
{
  return Tt_;
}
inline mousse::scalar mousse::liquidProperties::Pt() const
{
  return Pt_;
}
inline mousse::scalar mousse::liquidProperties::Tb() const
{
  return Tb_;
}
inline mousse::scalar mousse::liquidProperties::dipm() const
{
  return dipm_;
}
inline mousse::scalar mousse::liquidProperties::omega() const
{
  return omega_;
}
inline mousse::scalar mousse::liquidProperties::delta() const
{
  return delta_;
}
#endif
