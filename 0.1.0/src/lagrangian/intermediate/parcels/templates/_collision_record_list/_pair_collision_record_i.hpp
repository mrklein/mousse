// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

// Member Functions 
template<class Type>
inline bool mousse::PairCollisionRecord<Type>::match
(
  label queryOrigProcOfOther,
  label queryOrigIdOfOther
) const
{
  return
  (
    queryOrigProcOfOther == origProcOfOther()
  && queryOrigIdOfOther == origIdOfOther()
  );
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
  return
  (
    a.origProcOfOther_ == b.origProcOfOther_
  && a.origIdOfOther_ == b.origIdOfOther_
  && a.data_ == b.data_
  );
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
