// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_mppic_parcel.hpp"
#include "iostreams.hpp"
#include "iofield.hpp"
// Static Data Members
template<class ParcelType>
mousse::string mousse::MPPICParcel<ParcelType>::propertyList_ =
  mousse::MPPICParcel<ParcelType>::propertyList();
template<class ParcelType>
const std::size_t mousse::MPPICParcel<ParcelType>::sizeofFields_
(
  sizeof(MPPICParcel<ParcelType>) - sizeof(ParcelType)
);
// Constructors 
template<class ParcelType>
mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType(mesh, is, readFields),
  UCorrect_(vector::zero)
{
  if (readFields)
  {
    if (is.format() == IOstream::ASCII)
    {
      is >> UCorrect_;
    }
    else
    {
      is.read(reinterpret_cast<char*>(&UCorrect_), sizeofFields_);
    }
  }
  is.check
  (
    "MPPICParcel<ParcelType>::Collisions"
    "(const polyMesh&, Istream&, bool)"
  );
}
template<class ParcelType>
template<class CloudType>
void mousse::MPPICParcel<ParcelType>::readFields(CloudType& c)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c);
  IOField<vector> UCorrect(c.fieldIOobject("UCorrect", IOobject::MUST_READ));
  c.checkFieldIOobject(c, UCorrect);
  label i = 0;
  FOR_ALL_ITER(typename CloudType, c, iter)
  {
    MPPICParcel<ParcelType>& p = iter();
    p.UCorrect_ = UCorrect[i];
    i++;
  }
}
template<class ParcelType>
template<class CloudType>
void mousse::MPPICParcel<ParcelType>::writeFields(const CloudType& c)
{
  ParcelType::writeFields(c);
  label np =  c.size();
  IOField<vector>
    UCorrect(c.fieldIOobject("UCorrect", IOobject::NO_READ), np);
  label i = 0;
  FOR_ALL_CONST_ITER(typename CloudType, c, iter)
  {
    const MPPICParcel<ParcelType>& p = iter();
    UCorrect[i] = p.UCorrect();
    i++;
  }
  UCorrect.write();
}
// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const MPPICParcel<ParcelType>& p
)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const ParcelType&>(p)
      << token::SPACE << p.UCorrect();
  }
  else
  {
    os  << static_cast<const ParcelType&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.UCorrect_),
      MPPICParcel<ParcelType>::sizeofFields_
    );
  }
  os.check
  (
    "Ostream& operator<<(Ostream&, const MPPICParcel<ParcelType>&)"
  );
  return os;
}
