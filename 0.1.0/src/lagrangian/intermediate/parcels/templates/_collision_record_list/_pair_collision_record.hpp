// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PairCollisionRecord
// Description
//   Record of a collision between the particle holding the record and
//   the particle with the stored id.
//   The access status of the record is to be coded in the
//   origProcOfOther data member.  The actual processor is offset by
//   +1.  A negative value means that the record has not been accessed,
//   positive means that it has.
// SourceFiles
//   _pair_collision_record.cpp
//   _pair_collision_record_io.cpp
#ifndef _pair_collision_record_hpp_
#define _pair_collision_record_hpp_
#include "label.hpp"
#include "vector.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type>
class PairCollisionRecord;
template<class Type>
inline bool operator==
(
  const PairCollisionRecord<Type>&,
  const PairCollisionRecord<Type>&
);
template<class Type>
inline bool operator!=
(
  const PairCollisionRecord<Type>&,
  const PairCollisionRecord<Type>&
);
template<class Type>
Istream& operator>>(Istream&, PairCollisionRecord<Type>&);
template<class Type>
Ostream& operator<<(Ostream&, const PairCollisionRecord<Type>&);
template<class Type>
class PairCollisionRecord
{
  // Private data
    //- Originating processor id of other collision partner
    label origProcOfOther_;
    //- Local particle id on originating processor of other
    //  collision partner
    label origIdOfOther_;
    //- Collision data, stored as if the storing particle was the
    //  first particle (particle A) in the collision.
    Type data_;
public:
  // Constructors
    //- Construct null
    PairCollisionRecord();
    //- Construct from components
    PairCollisionRecord
    (
      bool accessed,
      label origProcOfOther,
      label origIdOfOther,
      const Type& data = pTraits<Type>::zero
    );
    //- Construct from Istream
    PairCollisionRecord(Istream&);
    //- Construct as copy
    PairCollisionRecord(const PairCollisionRecord&);
  //- Destructor
    ~PairCollisionRecord();
  // Member Functions
    // Access
      //- Return the origProcOfOther data
      inline label origProcOfOther() const;
      //- Return the origIdOfOther data
      inline label origIdOfOther() const;
      //- Return access to the collision data
      inline const Type& collisionData() const;
      //- Return access to the collision data
      inline Type& collisionData();
    // Check
      inline bool match
      (
        label queryOrigProcOfOther,
        label queryOrigIdOfOther
      ) const;
      //- Return the accessed status of the record
      inline bool accessed() const;
    // Edit
      //- Set the accessed property of the record to accessed
      inline void setAccessed();
      //- Set the accessed property of the record to unaccessed
      inline void setUnaccessed();
  // Member Operators
    void operator=(const PairCollisionRecord&);
  // Friend Operators
    friend bool operator== <Type>
    (
      const PairCollisionRecord<Type>&,
      const PairCollisionRecord<Type>&
    );
    friend bool operator!= <Type>
    (
      const PairCollisionRecord<Type>&,
      const PairCollisionRecord<Type>&
    );
  // IOstream Operators
    friend Istream& operator>> <Type>
    (
      Istream&,
      PairCollisionRecord<Type>&
    );
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const PairCollisionRecord<Type>&
    );
};
}  // namespace mousse

// Member Functions 
template<class Type>
inline bool mousse::PairCollisionRecord<Type>::match
(
  label queryOrigProcOfOther,
  label queryOrigIdOfOther
) const
{
  return (queryOrigProcOfOther == origProcOfOther()
          && queryOrigIdOfOther == origIdOfOther());
}
template<class Type>
inline mousse::label mousse::PairCollisionRecord<Type>::origProcOfOther() const
{
  return mag(origProcOfOther_) - 1;
}
template<class Type>
inline mousse::label mousse::PairCollisionRecord<Type>::origIdOfOther() const
{
  return origIdOfOther_;
}
template<class Type>
inline const Type&
mousse::PairCollisionRecord<Type>::collisionData() const
{
  return data_;
}
template<class Type>
inline Type& mousse::PairCollisionRecord<Type>::collisionData()
{
  return data_;
}
template<class Type>
inline bool mousse::PairCollisionRecord<Type>::accessed() const
{
  return pos(origProcOfOther_);
}
template<class Type>
inline void mousse::PairCollisionRecord<Type>::setAccessed()
{
  origProcOfOther_ = origProcOfOther() + 1;
}
template<class Type>
inline void mousse::PairCollisionRecord<Type>::setUnaccessed()
{
  origProcOfOther_ = -(origProcOfOther() + 1);
}
// Friend Operators
template<class Type>
inline bool mousse::operator==
(
  const PairCollisionRecord<Type>& a,
  const PairCollisionRecord<Type>& b
)
{
  return (a.origProcOfOther_ == b.origProcOfOther_
          && a.origIdOfOther_ == b.origIdOfOther_
          && a.data_ == b.data_);
}
template<class Type>
inline bool mousse::operator!=
(
  const PairCollisionRecord<Type>& a,
  const PairCollisionRecord<Type>& b
)
{
  return !(a == b);
}
#ifdef NoRepository
#   include "_pair_collision_record.cpp"
#endif
#endif
