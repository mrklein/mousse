// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactingMultiphaseParcelInjectionData
// Description
//   Container class to provide injection data for reacting multiphase parcels
// SourceFiles
//   reacting_multiphase_parcel_injection_data.cpp
#ifndef reacting_multiphase_parcel_injection_data_hpp_
#define reacting_multiphase_parcel_injection_data_hpp_
#include "reacting_parcel_injection_data.hpp"
namespace mousse
{
// Forward declaration of classes
class reactingMultiphaseParcelInjectionData;
// Forward declaration of friend functions
Ostream& operator<<
(
  Ostream&,
  const reactingMultiphaseParcelInjectionData&
);
Istream& operator>>
(
  Istream&,
  reactingMultiphaseParcelInjectionData&
);
class reactingMultiphaseParcelInjectionData
:
  public reactingParcelInjectionData
{
protected:
  // Parcel properties
    //- List of gaseous mass fractions
    scalarList YGas_;
    //- List of liquid mass fractions
    scalarList YLiquid_;
    //- List of solid mass fractions
    scalarList YSolid_;
public:
  //- Runtime type information
  TYPE_NAME("reactingMultiphaseParcelInjectionData");
  // Constructors
    //- Null constructor
    reactingMultiphaseParcelInjectionData();
    //- Construct from dictionary
    reactingMultiphaseParcelInjectionData(const dictionary& dict);
    //- Construct from Istream
    reactingMultiphaseParcelInjectionData(Istream& is);
  //-Destructor
  virtual ~reactingMultiphaseParcelInjectionData();
  // Access
    //- Return const access to the list of gaseous mass fractions
    inline const scalarList& YGas() const;
    //- Return const access to the list of liquid mass fractions
    inline const scalarList& YLiquid() const;
    //- Return const access to the list of solid mass fractions
    inline const scalarList& YSolid() const;
  // Edit
    //- Return access to the gaseous mass fractions
    inline scalarList& YGas();
    //- Return access to the liquid mass fractions
    inline scalarList& YLiquid();
    //- Return access to the solid mass fractions
    inline scalarList& YSolid();
  // I-O
    //- Ostream operator
    friend Ostream& operator<<
    (
      Ostream& os,
      const reactingMultiphaseParcelInjectionData& data
    );
    //- Istream operator
    friend Istream& operator>>
    (
      Istream& is,
      reactingMultiphaseParcelInjectionData& data
    );
};
}  // namespace mousse

inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YGas() const
{
  return YGas_;
}
inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YLiquid() const
{
  return YLiquid_;
}
inline const mousse::scalarList&
mousse::reactingMultiphaseParcelInjectionData::YSolid() const
{
  return YSolid_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YGas()
{
  return YGas_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YLiquid()
{
  return YLiquid_;
}
inline mousse::scalarList& mousse::reactingMultiphaseParcelInjectionData::YSolid()
{
  return YSolid_;
}
#endif
