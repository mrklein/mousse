// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingParcelInjectionData
// Description
//   Container class to provide injection data for reacting parcels
// SourceFiles
//   reacting_parcel_injection_data.cpp
#ifndef reacting_parcel_injection_data_hpp_
#define reacting_parcel_injection_data_hpp_
#include "thermo_parcel_injection_data.hpp"
#include "scalar_list.hpp"
namespace mousse
{
// Forward declaration of classes
class reactingParcelInjectionData;
// Forward declaration of friend functions
Ostream& operator<<
(
  Ostream&,
  const reactingParcelInjectionData&
);
Istream& operator>>
(
  Istream&,
  reactingParcelInjectionData&
);
class reactingParcelInjectionData
:
  public thermoParcelInjectionData
{
protected:
  // Parcel properties
    //- List of mass fractions
    scalarList Y_;
public:
  //- Runtime type information
  TypeName("reactingParcelInjectionData");
  // Constructors
    //- Null constructor
    reactingParcelInjectionData();
    //- Construct from dictionary
    reactingParcelInjectionData(const dictionary& dict);
    //- Construct from Istream
    reactingParcelInjectionData(Istream& is);
  //-Destructor
  virtual ~reactingParcelInjectionData();
  // Access
    //- Return const access to the list of mass fractions
    inline const scalarList& Y() const;
  // Edit
    //- Return access to the mass fractions
    inline scalarList& Y();
  // I-O
    //- Ostream operator
    friend Ostream& operator<<
    (
      Ostream& os,
      const reactingParcelInjectionData& data
    );
    //- Istream operator
    friend Istream& operator>>
    (
      Istream& is,
      reactingParcelInjectionData& data
    );
};
}  // namespace mousse
#include "reacting_parcel_injection_data_i.hpp"
#endif
