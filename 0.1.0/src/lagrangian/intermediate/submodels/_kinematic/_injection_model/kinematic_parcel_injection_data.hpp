#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_KINEMATIC_PARCEL_INJECTION_DATA_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TINJECTION_MODEL_KINEMATIC_PARCEL_INJECTION_DATA_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::kinematicParcelInjectionData
// Description
//   Container class to provide injection data for kinematic parcels
// SourceFiles
//   kinematic_parcel_injection_data.cpp
#include "dictionary.hpp"
#include "vector.hpp"
#include "point.hpp"
namespace mousse
{
// Forward declaration of classes
class kinematicParcelInjectionData;
// Forward declaration of friend functions
Ostream& operator<<
(
  Ostream&,
  const kinematicParcelInjectionData&
);
Istream& operator>>
(
  Istream&,
  kinematicParcelInjectionData&
);
class kinematicParcelInjectionData
{
protected:
  // Parcel properties
    //- Position [m]
    point x_;
    //- Velocity [m/s]
    vector U_;
    //- Diameter [m]
    scalar d_;
    //- Density [kg/m3]
    scalar rho_;
    //- Mass flow rate [kg/s]
    scalar mDot_;
public:
  //- Runtime type information
  TYPE_NAME("kinematicParcelInjectionData");
  // Constructors
    //- Null constructor
    kinematicParcelInjectionData();
    //- Construct from dictionary
    kinematicParcelInjectionData(const dictionary& dict);
    //- Construct from Istream
    kinematicParcelInjectionData(Istream& is);
  //-Destructor
  virtual ~kinematicParcelInjectionData();
  // Access
    //- Return const access to the position
    inline const point& x() const;
    //- Return const access to the velocity
    inline const vector& U() const;
    //- Return const access to the diameter
    inline scalar d() const;
    //- Return const access to the density
    inline scalar rho() const;
    //- Return const access to the mass flow rate
    inline scalar mDot() const;
  // Edit
    //- Return access to the position
    inline point& x();
    //- Return access to the velocity
    inline vector& U();
    //- Return access to the diameter
    inline scalar& d();
    //- Return access to the density
    inline scalar& rho();
    //- Return access to the mass flow rate
    inline scalar& mDot();
  // Friend Operators
    friend bool operator==
    (
      const kinematicParcelInjectionData& /*a*/,
      const kinematicParcelInjectionData& /*b*/
    )
    {
      NOT_IMPLEMENTED
      (
        "operator=="
        "("
          "const kinematicParcelInjectionData&, "
          "const kinematicParcelInjectionData&"
        ")"
      );
      return false;
    }
    friend bool operator!=
    (
      const kinematicParcelInjectionData& /*a*/,
      const kinematicParcelInjectionData& /*b*/
    )
    {
      NOT_IMPLEMENTED
      (
        "operator=="
        "("
          "const kinematicParcelInjectionData&, "
          "const kinematicParcelInjectionData&"
        ")"
      );
      return false;
    }
  // I-O
    //- Ostream operator
    friend Ostream& operator<<
    (
      Ostream& os,
      const kinematicParcelInjectionData& data
    );
    //- Istream operator
    friend Istream& operator>>
    (
      Istream& is,
      kinematicParcelInjectionData& data
    );
};
}  // namespace mousse

inline const mousse::point& mousse::kinematicParcelInjectionData::x() const
{
  return x_;
}
inline const mousse::vector& mousse::kinematicParcelInjectionData::U() const
{
  return U_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::d() const
{
  return d_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::rho() const
{
  return rho_;
}
inline mousse::scalar mousse::kinematicParcelInjectionData::mDot() const
{
  return mDot_;
}
inline mousse::point& mousse::kinematicParcelInjectionData::x()
{
  return x_;
}
inline mousse::vector& mousse::kinematicParcelInjectionData::U()
{
  return U_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::d()
{
  return d_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::rho()
{
  return rho_;
}
inline mousse::scalar& mousse::kinematicParcelInjectionData::mDot()
{
  return mDot_;
}
#endif
