// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_pair_collision_record.hpp"
// Constructors 
template<class Type>
mousse::PairCollisionRecord<Type>::PairCollisionRecord()
:
  origProcOfOther_(0),
  origIdOfOther_(-1),
  data_(pTraits<Type>::zero)
{}
template<class Type>
mousse::PairCollisionRecord<Type>::PairCollisionRecord
(
  bool accessed,
  label origProcOfOther,
  label origIdOfOther,
  const Type& data
)
:
  origProcOfOther_(origProcOfOther + 1),
  origIdOfOther_(origIdOfOther),
  data_(data)
{
  // Default assignment to origProcOfOther_ assumes accessed is true
  if (!accessed)
  {
    setUnaccessed();
  }
}
template<class Type>
mousse::PairCollisionRecord<Type>::PairCollisionRecord
(
  const PairCollisionRecord<Type>& pCR
)
:
  origProcOfOther_(pCR.origProcOfOther_),
  origIdOfOther_(pCR.origIdOfOther_),
  data_(pCR.data_)
{}
// Destructor 
template<class Type>
mousse::PairCollisionRecord<Type>::~PairCollisionRecord()
{}
// Member Operators 
template<class Type>
void mousse::PairCollisionRecord<Type>::operator=
(
  const PairCollisionRecord<Type>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_ERROR_IN
    (
      "mousse::PairCollisionRecord<Type>::operator="
      "(const mousse::PairCollisionRecord<Type>&)"
    )
      << "Attempted assignment to self"
      << abort(FatalError);
  }
  origProcOfOther_ = rhs.origProcOfOther_;
  origIdOfOther_ = rhs.origIdOfOther_;
  data_ = rhs.data_;
}
//  IOStream operators
#include "_pair_collision_record_io.cpp"
