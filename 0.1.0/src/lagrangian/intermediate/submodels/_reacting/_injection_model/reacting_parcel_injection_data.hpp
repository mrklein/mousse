#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_REACTING_PARCEL_INJECTION_DATA_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_TINJECTION_MODEL_REACTING_PARCEL_INJECTION_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingParcelInjectionData
// Description
//   Container class to provide injection data for reacting parcels

#include "thermo_parcel_injection_data.hpp"
#include "scalar_list.hpp"


namespace mousse {

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
  TYPE_NAME("reactingParcelInjectionData");
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


inline const mousse::scalarList& mousse::reactingParcelInjectionData::Y() const
{
  return Y_;
}


inline mousse::scalarList& mousse::reactingParcelInjectionData::Y()
{
  return Y_;
}

#endif
