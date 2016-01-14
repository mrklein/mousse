// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermoParcelInjectionData
// Description
//   Container class to provide injection data for thermodynamic parcels
// SourceFiles
//   thermo_parcel_injection_data.cpp
#ifndef thermo_parcel_injection_data_hpp_
#define thermo_parcel_injection_data_hpp_
#include "kinematic_parcel_injection_data.hpp"
namespace mousse
{
// Forward declaration of classes
class thermoParcelInjectionData;
// Forward declaration of friend functions
Ostream& operator<<
(
  Ostream&,
  const thermoParcelInjectionData&
);
Istream& operator>>
(
  Istream&,
  thermoParcelInjectionData&
);
class thermoParcelInjectionData
:
  public kinematicParcelInjectionData
{
protected:
  // Parcel properties
    //- Temperature [K]
    scalar T_;
    //- Specific heat capacity [J/kg/K]
    scalar Cp_;
public:
  //- Runtime type information
  TYPE_NAME("thermoParcelInjectionData");
  // Constructors
    //- Null constructor
    thermoParcelInjectionData();
    //- Construct from dictionary
    thermoParcelInjectionData(const dictionary& dict);
    //- Construct from Istream
    thermoParcelInjectionData(Istream& is);
  //-Destructor
  virtual ~thermoParcelInjectionData();
  // Access
    //- Return const access to the temperature
    inline scalar T() const;
    //- Return const access to the specific heat capacity
    inline scalar Cp() const;
  // Edit
    //- Return access to the temperature
    inline scalar& T();
    //- Return access to the specific heat capacity
    inline scalar& Cp();
  // I-O
    //- Ostream operator
    friend Ostream& operator<<
    (
      Ostream& os,
      const thermoParcelInjectionData& data
    );
    //- Istream operator
    friend Istream& operator>>
    (
      Istream& is,
      thermoParcelInjectionData& data
    );
};
}  // namespace mousse
#include "thermo_parcel_injection_data_i.hpp"
#endif
