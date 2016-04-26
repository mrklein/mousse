#ifndef THERMOPHYSICAL_MODELS_PROPERTIES_SOLID_PROPERTIES_SOLID_PROPERTIES_SOLID_PROPERTIES_HPP_
#define THERMOPHYSICAL_MODELS_PROPERTIES_SOLID_PROPERTIES_SOLID_PROPERTIES_SOLID_PROPERTIES_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidProperties
// Description
//   The thermophysical properties of a solid

#include "type_info.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "dictionary.hpp"
#include "specie.hpp"


namespace mousse {

class solidProperties;

Ostream& operator<<
(
  Ostream&,
  const solidProperties&
);


class solidProperties
{
  // Private data
    //- Density [kg/m3]
    scalar rho_;
    //- Specific heat capacity [J/(kg.K)]
    scalar Cp_;
    //- Thermal conductivity [W/(m.K)]
    scalar K_;
    //- Heat of formation [J/kg]
    scalar Hf_;
    //- Emissivity
    scalar emissivity_;
public:
  //- Runtime type information
  TYPE_NAME("solidProperties");
  // Declare run-time constructor selection tables
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidProperties,
    ,
    (),
    ()
  );
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidProperties,
    Istream,
    (Istream& is),
    (is)
  );
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    solidProperties,
    dictionary,
    (const dictionary& dict),
    (dict)
  );
  // Constructors
    //- Construct from components
    solidProperties
    (
      scalar rho,
      scalar Cp,
      scalar K,
      scalar Hf,
      scalar emissivity
    );
    //- Construct from Istream
    solidProperties(Istream& is);
    //- Construct from dictionary
    solidProperties(const dictionary& dict);
    //- Construct copy
    solidProperties(const solidProperties& s);
    //- Construct and return clone
    virtual autoPtr<solidProperties> clone() const
    {
      return autoPtr<solidProperties>{new solidProperties(*this)};
    }
  // Selectors
    //- Return a pointer to a new solidProperties created from input
    static autoPtr<solidProperties> New(Istream& is);
    //- Return a pointer to a new solidProperties created from dictionary
    static autoPtr<solidProperties> New(const dictionary& dict);
  //- Destructor
  virtual ~solidProperties()
  {}
  // Member Functions
    // Physical constants which define the solidProperties
      //- Density [kg/m3]
      inline scalar rho() const;
      //- Specific heat capacity [J/(kg.K)]
      inline scalar Cp() const;
      //- Thermal conductivity [W/(m.K)]
      inline scalar K() const;
      //- Heat of formation [J/kg]
      inline scalar Hf() const;
      //- Sensible enthalpy - reference to Tstd [J/kg]
      inline scalar Hs(const scalar T) const;
      //- Emissivity []
      inline scalar emissivity() const;
    // I-O
      //- Write the solidProperties properties
      virtual void writeData(Ostream& os) const;
    // Ostream Operator
    friend Ostream& operator<<(Ostream& os, const solidProperties& s);
};

}  // namespace mousse


inline mousse::scalar mousse::solidProperties::rho() const
{
  return rho_;
}


inline mousse::scalar mousse::solidProperties::Cp() const
{
  return Cp_;
}


inline mousse::scalar mousse::solidProperties::K() const
{
  return K_;
}


inline mousse::scalar mousse::solidProperties::Hf() const
{
  return Hf_;
}


inline mousse::scalar mousse::solidProperties::Hs(const scalar T) const
{
  return Cp_*(T - Tstd);
}


inline mousse::scalar mousse::solidProperties::emissivity() const
{
  return emissivity_;
}

#endif
