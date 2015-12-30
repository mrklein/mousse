// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_dsmc_parcel.hpp"
#include "iostreams.hpp"
#include "iofield.hpp"
#include "_cloud.hpp"
// Static Data Members
template<class ParcelType>
const std::size_t mousse::DSMCParcel<ParcelType>::sizeofFields_
(
  sizeof(DSMCParcel<ParcelType>) - sizeof(ParcelType)
);
// Constructors 
template<class ParcelType>
mousse::DSMCParcel<ParcelType>::DSMCParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType(mesh, is, readFields),
  U_(vector::zero),
  Ei_(0.0),
  typeId_(-1)
{
  if (readFields)
  {
    if (is.format() == IOstream::ASCII)
    {
      is >> U_;
      Ei_ = readScalar(is);
      typeId_ = readLabel(is);
    }
    else
    {
      is.read(reinterpret_cast<char*>(&U_), sizeofFields_);
    }
  }
  // Check state of Istream
  is.check
  (
    "DSMCParcel<ParcelType>::DSMCParcel"
    "(const Cloud<ParcelType>&, Istream&, bool)"
  );
}
template<class ParcelType>
void mousse::DSMCParcel<ParcelType>::readFields(Cloud<DSMCParcel<ParcelType> >& c)
{
  if (!c.size())
  {
    return;
  }
  ParcelType::readFields(c);
  IOField<vector> U(c.fieldIOobject("U", IOobject::MUST_READ));
  c.checkFieldIOobject(c, U);
  IOField<scalar> Ei(c.fieldIOobject("Ei", IOobject::MUST_READ));
  c.checkFieldIOobject(c, Ei);
  IOField<label> typeId(c.fieldIOobject("typeId", IOobject::MUST_READ));
  c.checkFieldIOobject(c, typeId);
  label i = 0;
  forAllIter(typename Cloud<DSMCParcel<ParcelType> >, c, iter)
  {
    DSMCParcel<ParcelType>& p = iter();
    p.U_ = U[i];
    p.Ei_ = Ei[i];
    p.typeId_ = typeId[i];
    i++;
  }
}
template<class ParcelType>
void mousse::DSMCParcel<ParcelType>::writeFields
(
  const Cloud<DSMCParcel<ParcelType> >& c
)
{
  ParcelType::writeFields(c);
  label np =  c.size();
  IOField<vector> U(c.fieldIOobject("U", IOobject::NO_READ), np);
  IOField<scalar> Ei(c.fieldIOobject("Ei", IOobject::NO_READ), np);
  IOField<label> typeId(c.fieldIOobject("typeId", IOobject::NO_READ), np);
  label i = 0;
  forAllConstIter(typename Cloud<DSMCParcel<ParcelType> >, c, iter)
  {
    const DSMCParcel<ParcelType>& p = iter();
    U[i] = p.U();
    Ei[i] = p.Ei();
    typeId[i] = p.typeId();
    i++;
  }
  U.write();
  Ei.write();
  typeId.write();
}
// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const DSMCParcel<ParcelType>& p
)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const ParcelType& >(p)
      << token::SPACE << p.U()
      << token::SPACE << p.Ei()
      << token::SPACE << p.typeId();
  }
  else
  {
    os  << static_cast<const ParcelType& >(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.U_),
      DSMCParcel<ParcelType>::sizeofFields_
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const DSMCParcel<ParcelType>&)"
  );
  return os;
}
