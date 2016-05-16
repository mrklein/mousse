// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_colliding_parcel.hpp"
#include "iostreams.hpp"
#include "iofield.hpp"


// Static Data Members
template<class ParcelType>
mousse::string mousse::CollidingParcel<ParcelType>::propertyList_ =
  mousse::CollidingParcel<ParcelType>::propertyList();

template<class ParcelType>
const std::size_t mousse::CollidingParcel<ParcelType>::sizeofFields_
(
  offsetof(CollidingParcel<ParcelType>, collisionRecords_)
  - offsetof(CollidingParcel<ParcelType>, f_)
);


// Constructors 
template<class ParcelType>
mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  ParcelType{mesh, is, readFields},
  f_{vector::zero},
  angularMomentum_{vector::zero},
  torque_{vector::zero},
  collisionRecords_{}
{
  if (readFields) {
    if (is.format() == IOstream::ASCII) {
      is >> f_;
      is >> angularMomentum_;
      is >> torque_;
    } else {
      is.read(reinterpret_cast<char*>(&f_), sizeofFields_);
    }
    is >> collisionRecords_;
  }
  // Check state of Istream
  is.check
  (
    "CollidingParcel<ParcelType>::Collisions"
    "(const polyMesh&, Istream&, bool)"
  );
}


template<class ParcelType>
template<class CloudType>
void mousse::CollidingParcel<ParcelType>::readFields(CloudType& c)
{
  if (!c.size()) {
    return;
  }
  ParcelType::readFields(c);
  IOField<vector> f{c.fieldIOobject("f", IOobject::MUST_READ)};
  c.checkFieldIOobject(c, f);
  IOField<vector> angularMomentum
  {
    c.fieldIOobject("angularMomentum", IOobject::MUST_READ)
  };
  c.checkFieldIOobject(c, angularMomentum);
  IOField<vector> torque{c.fieldIOobject("torque", IOobject::MUST_READ)};
  c.checkFieldIOobject(c, torque);
  labelFieldCompactIOField collisionRecordsPairAccessed
  {
    c.fieldIOobject("collisionRecordsPairAccessed", IOobject::MUST_READ)
  };
  c.checkFieldFieldIOobject(c, collisionRecordsPairAccessed);
  labelFieldCompactIOField collisionRecordsPairOrigProcOfOther
  {
    c.fieldIOobject
    (
      "collisionRecordsPairOrigProcOfOther",
      IOobject::MUST_READ
    )
  };
  c.checkFieldFieldIOobject(c, collisionRecordsPairOrigProcOfOther);
  labelFieldCompactIOField collisionRecordsPairOrigIdOfOther
  {
    c.fieldIOobject
    (
      "collisionRecordsPairOrigIdOfOther",
      IOobject::MUST_READ
    )
  };
  c.checkFieldFieldIOobject(c, collisionRecordsPairOrigProcOfOther);
  pairDataFieldCompactIOField collisionRecordsPairData
  {
    c.fieldIOobject("collisionRecordsPairData", IOobject::MUST_READ)
  };
  c.checkFieldFieldIOobject(c, collisionRecordsPairData);
  labelFieldCompactIOField collisionRecordsWallAccessed
  {
    c.fieldIOobject("collisionRecordsWallAccessed", IOobject::MUST_READ)
  };
  c.checkFieldFieldIOobject(c, collisionRecordsWallAccessed);
  vectorFieldCompactIOField collisionRecordsWallPRel
  {
    c.fieldIOobject("collisionRecordsWallPRel", IOobject::MUST_READ)
  };
  c.checkFieldFieldIOobject(c, collisionRecordsWallPRel);
  wallDataFieldCompactIOField collisionRecordsWallData
  {
    c.fieldIOobject("collisionRecordsWallData", IOobject::MUST_READ)
  };
  c.checkFieldFieldIOobject(c, collisionRecordsWallData);
  label i = 0;
  FOR_ALL_ITER(typename CloudType, c, iter) {
    CollidingParcel<ParcelType>& p = iter();
    p.f_ = f[i];
    p.angularMomentum_ = angularMomentum[i];
    p.torque_ = torque[i];
    p.collisionRecords_ =
      collisionRecordList
      (
        collisionRecordsPairAccessed[i],
        collisionRecordsPairOrigProcOfOther[i],
        collisionRecordsPairOrigIdOfOther[i],
        collisionRecordsPairData[i],
        collisionRecordsWallAccessed[i],
        collisionRecordsWallPRel[i],
        collisionRecordsWallData[i]
      );
    i++;
  }
}


template<class ParcelType>
template<class CloudType>
void mousse::CollidingParcel<ParcelType>::writeFields(const CloudType& c)
{
  ParcelType::writeFields(c);
  label np =  c.size();
  IOField<vector> f{c.fieldIOobject("f", IOobject::NO_READ), np};
  IOField<vector> angularMomentum
  {
    c.fieldIOobject("angularMomentum", IOobject::NO_READ),
    np
  };
  IOField<vector> torque{c.fieldIOobject("torque", IOobject::NO_READ), np};
  labelFieldCompactIOField collisionRecordsPairAccessed
  {
    c.fieldIOobject("collisionRecordsPairAccessed", IOobject::NO_READ),
    np
  };
  labelFieldCompactIOField collisionRecordsPairOrigProcOfOther
  {
    c.fieldIOobject
    (
      "collisionRecordsPairOrigProcOfOther",
      IOobject::NO_READ
    ),
    np
  };
  labelFieldCompactIOField collisionRecordsPairOrigIdOfOther
  {
    c.fieldIOobject("collisionRecordsPairOrigIdOfOther", IOobject::NO_READ),
    np
  };
  pairDataFieldCompactIOField collisionRecordsPairData
  {
    c.fieldIOobject("collisionRecordsPairData", IOobject::NO_READ),
    np
  };
  labelFieldCompactIOField collisionRecordsWallAccessed
  {
    c.fieldIOobject("collisionRecordsWallAccessed", IOobject::NO_READ),
    np
  };
  vectorFieldCompactIOField collisionRecordsWallPRel
  {
    c.fieldIOobject("collisionRecordsWallPRel", IOobject::NO_READ),
    np
  };
  wallDataFieldCompactIOField collisionRecordsWallData
  {
    c.fieldIOobject("collisionRecordsWallData", IOobject::NO_READ),
    np
  };
  label i = 0;
  FOR_ALL_CONST_ITER(typename CloudType, c, iter) {
    const CollidingParcel<ParcelType>& p = iter();
    f[i] = p.f();
    angularMomentum[i] = p.angularMomentum();
    torque[i] = p.torque();
    collisionRecordsPairAccessed[i] = p.collisionRecords().pairAccessed();
    collisionRecordsPairOrigProcOfOther[i] =
      p.collisionRecords().pairOrigProcOfOther();
    collisionRecordsPairOrigIdOfOther[i] =
      p.collisionRecords().pairOrigIdOfOther();
    collisionRecordsPairData[i] = p.collisionRecords().pairData();
    collisionRecordsWallAccessed[i] = p.collisionRecords().wallAccessed();
    collisionRecordsWallPRel[i] = p.collisionRecords().wallPRel();
    collisionRecordsWallData[i] = p.collisionRecords().wallData();
    i++;
  }
  f.write();
  angularMomentum.write();
  torque.write();
  collisionRecordsPairAccessed.write();
  collisionRecordsPairOrigProcOfOther.write();
  collisionRecordsPairOrigIdOfOther.write();
  collisionRecordsPairData.write();
  collisionRecordsWallAccessed.write();
  collisionRecordsWallPRel.write();
  collisionRecordsWallData.write();
}


// IOstream Operators 
template<class ParcelType>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CollidingParcel<ParcelType>& p
)
{
  if (os.format() == IOstream::ASCII) {
    os << static_cast<const ParcelType&>(p)
      << token::SPACE << p.f_
      << token::SPACE << p.angularMomentum_
      << token::SPACE << p.torque_
      << token::SPACE << p.collisionRecords_;
  } else {
    os << static_cast<const ParcelType&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.f_),
      CollidingParcel<ParcelType>::sizeofFields_
    );
    os << p.collisionRecords();
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const CollidingParcel<ParcelType>&)"
  );
  return os;
}

