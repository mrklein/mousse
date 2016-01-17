// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WallCollisionRecord
// Description
//   Record of a collision between the particle holding the record and
//   a wall face at the position relative to the centre of the particle.
// SourceFiles
//   _wall_collision_record.cpp
//   _wall_collision_record_io.cpp
#ifndef _wall_collision_record_hpp_
#define _wall_collision_record_hpp_
#include "vector.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type>
class WallCollisionRecord;
template<class Type>
inline bool operator==
(
  const WallCollisionRecord<Type>&,
  const WallCollisionRecord<Type>&
);
template<class Type>
inline bool operator!=
(
  const WallCollisionRecord<Type>&,
  const WallCollisionRecord<Type>&
);
template<class Type>
Istream& operator>>(Istream&, WallCollisionRecord<Type>&);
template<class Type>
Ostream& operator<<(Ostream&, const WallCollisionRecord<Type>&);
template<class Type>
class WallCollisionRecord
{
  // Private data
    // //- Recording whether or not this record has been accessed
    bool accessed_;
    //- The position of wall impact relative to the particle centre
    vector pRel_;
    //- Collision data, stored as if the storing particle was the
    //  first particle (particle A) in the collision.
    Type data_;
public:
  // Static data members
    //- Tolerance for detecting seriously erroneous wall matches
    static const scalar errorCosAngle;
  // Constructors
    //- Construct null
    WallCollisionRecord();
    //- Construct from components
    WallCollisionRecord
    (
      bool accessed,
      const vector& pRel,
      const Type& data = pTraits<Type>::zero
    );
    //- Construct from Istream
    WallCollisionRecord(Istream&);
    //- Construct as copy
    WallCollisionRecord(const WallCollisionRecord&);
  //- Destructor
  ~WallCollisionRecord();
  // Member Functions
    // Access
      //- Return the pRel data
      inline const vector& pRel() const;
      //- Return access to the collision data
      inline const Type& collisionData() const;
      //- Return access to the collision data
      inline Type& collisionData();
    // Check
      inline bool match(const vector& pRel, scalar radius);
      //- Return the accessed status of the record
      inline bool accessed() const;
    // Edit
      //- Set the accessed property of the record to accessed
      inline void setAccessed();
      //- Set the accessed property of the record to unaccessed
      inline void setUnaccessed();
  // Member Operators
    void operator=(const WallCollisionRecord&);
  // Friend Operators
    friend bool operator== <Type>
    (
      const WallCollisionRecord<Type>&,
      const WallCollisionRecord<Type>&
    );
    friend bool operator!= <Type>
    (
      const WallCollisionRecord<Type>&,
      const WallCollisionRecord<Type>&
    );
  // IOstream Operators
    friend Istream& operator>> <Type>
    (
      Istream&,
      WallCollisionRecord<Type>&
    );
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const WallCollisionRecord<Type>&
    );
};
}  // namespace mousse

// Member Functions 
template<class Type>
inline bool mousse::WallCollisionRecord<Type>::match
(
  const vector& pRel,
  scalar radius
)
{
  scalar magpRel_= mag(pRel_);
  scalar magpRel = mag(pRel);
  // Using the new data as the acceptance criterion
  scalar cosAcceptanceAngle = magpRel/radius;
  if (cosAcceptanceAngle > errorCosAngle)
  {
    Info<< "pRel_ " << pRel_ << " " << magpRel_ << nl
      << "pRel " << pRel << " " << magpRel << nl
      << "unit vector dot product " << (pRel & pRel_)/(magpRel_*magpRel)
      << nl << "cosAcceptanceAngle " << cosAcceptanceAngle
      << endl;
    FATAL_ERROR_IN
    (
      "inline bool mousse::WallCollisionRecord<Type>::match"
      "("
        "const vector& pRel,"
        "scalar radius"
      ") const"
    )
    << "Problem with matching WallCollisionRecord." << nl
    << "The given radius, " << radius << ", is smaller than distance "
    << "to the relative position of the WallInteractionSite, "
    << magpRel << nl
    << abort(FatalError);
  }
  // Are the test and recorded pRel (relative position vectors)
  // aligned to within the calculated tolerance?
  bool matched = (pRel & pRel_)/(magpRel_*magpRel) > cosAcceptanceAngle;
  if (matched)
  {
    pRel_ = pRel;
  }
  return matched;
}
template<class Type>
inline const mousse::vector&
mousse::WallCollisionRecord<Type>::pRel() const
{
  return pRel_;
}
template<class Type>
inline const Type&
mousse::WallCollisionRecord<Type>::collisionData() const
{
  return data_;
}
template<class Type>
inline Type& mousse::WallCollisionRecord<Type>::collisionData()
{
  return data_;
}
template<class Type>
inline bool mousse::WallCollisionRecord<Type>::accessed() const
{
  return accessed_;
}
template<class Type>
inline void mousse::WallCollisionRecord<Type>::setAccessed()
{
  accessed_ = true;
}
template<class Type>
inline void mousse::WallCollisionRecord<Type>::setUnaccessed()
{
  accessed_ = false;
}
// Friend Operators
template<class Type>
inline bool mousse::operator==
(
  const WallCollisionRecord<Type>& a,
  const WallCollisionRecord<Type>& b
)
{
  return (a.accessed_ == b.accessed_
          && a.pRel_ == b.pRel_
          && a.data_ == b.data_);
}
template<class Type>
inline bool mousse::operator!=
(
  const WallCollisionRecord<Type>& a,
  const WallCollisionRecord<Type>& b
)
{
  return !(a == b);
}
#ifdef NoRepository
#   include "_wall_collision_record.cpp"
#endif
#endif
