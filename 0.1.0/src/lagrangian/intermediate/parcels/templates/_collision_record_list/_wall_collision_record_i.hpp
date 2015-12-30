// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

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
    FatalErrorIn
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
  return
  (
    a.accessed_ == b.accessed_
  && a.pRel_ == b.pRel_
  && a.data_ == b.data_
  );
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
