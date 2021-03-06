// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_thermo_parcel.hpp"
#include "iostreams.hpp"


#pragma GCC diagnostic ignored "-Winvalid-offsetof"


// Static Data Members
template<class ParcelType>
mousse::string mousse::ThermoParcel<ParcelType>::propertyList_ =
  mousse::ThermoParcel<ParcelType>::propertyList();

template<class ParcelType>
const std::size_t mousse::ThermoParcel<ParcelType>::sizeofFields_
(
  offsetof(ThermoParcel<ParcelType>, Tc_) - offsetof(ThermoParcel<ParcelType>, T_)
);


// Constructors 
template<class ParcelType>
mousse::ThermoParcel<ParcelType>::ThermoParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType{mesh, is, readFields},
  T_{0.0},
  Cp_{0.0},
  Tc_{0.0},
  Cpc_{0.0}
{
  if (readFields) {
    if (is.format() == IOstream::ASCII) {
      T_ = readScalar(is);
      Cp_ = readScalar(is);
    } else {
      is.read(reinterpret_cast<char*>(&T_), sizeofFields_);
    }
  }
  // Check state of Istream
  is.check
  (
    "ThermoParcel::ThermoParcel(const polyMesh&, Istream&, bool)"
  );
}


template<class ParcelType>
template<class CloudType>
void mousse::ThermoParcel<ParcelType>::readFields(CloudType& c)
{
  if (!c.size()) {
    return;
  }
  ParcelType::readFields(c);
  IOField<scalar> T{c.fieldIOobject("T", IOobject::MUST_READ)};
  c.checkFieldIOobject(c, T);
  IOField<scalar> Cp{c.fieldIOobject("Cp", IOobject::MUST_READ)};
  c.checkFieldIOobject(c, Cp);
  label i = 0;
  FOR_ALL_ITER(typename Cloud<ThermoParcel<ParcelType>>, c, iter) {
    ThermoParcel<ParcelType>& p = iter();
    p.T_ = T[i];
    p.Cp_ = Cp[i];
    i++;
  }
}


template<class ParcelType>
template<class CloudType>
void mousse::ThermoParcel<ParcelType>::writeFields(const CloudType& c)
{
  ParcelType::writeFields(c);
  label np = c.size();
  IOField<scalar> T{c.fieldIOobject("T", IOobject::NO_READ), np};
  IOField<scalar> Cp{c.fieldIOobject("Cp", IOobject::NO_READ), np};
  label i = 0;
  FOR_ALL_CONST_ITER(typename Cloud<ThermoParcel<ParcelType>>, c, iter) {
    const ThermoParcel<ParcelType>& p = iter();
    T[i] = p.T_;
    Cp[i] = p.Cp_;
    i++;
  }
  T.write();
  Cp.write();
}


// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const ThermoParcel<ParcelType>& p
)
{
  if (os.format() == IOstream::ASCII) {
    os << static_cast<const ParcelType&>(p)
      << token::SPACE << p.T()
      << token::SPACE << p.Cp();
  } else {
    os << static_cast<const ParcelType&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.T_),
      ThermoParcel<ParcelType>::sizeofFields_
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const ThermoParcel<ParcelType>&)"
  );
  return os;
}

