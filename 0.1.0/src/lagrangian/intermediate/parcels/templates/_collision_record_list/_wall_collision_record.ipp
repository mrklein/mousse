// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_collision_record.hpp"


// Static Data Members
template<class Type>
const mousse::scalar mousse::WallCollisionRecord<Type>::errorCosAngle(1.0 + 1e-6);

// Constructors 
template<class Type>
mousse::WallCollisionRecord<Type>::WallCollisionRecord()
:
  accessed_{false},
  pRel_{},
  data_{pTraits<Type>::zero}
{}


template<class Type>
mousse::WallCollisionRecord<Type>::WallCollisionRecord
(
  bool accessed,
  const vector& pRel,
  const Type& data
)
:
  accessed_{accessed},
  pRel_{pRel},
  data_{data}
{}


template<class Type>
mousse::WallCollisionRecord<Type>::WallCollisionRecord
(
  const WallCollisionRecord<Type>& wCR
)
:
  accessed_{wCR.accessed_},
  pRel_{wCR.pRel_},
  data_{wCR.data_}
{}


// Destructor 
template<class Type>
mousse::WallCollisionRecord<Type>::~WallCollisionRecord()
{}


// Member Operators 
template<class Type>
void mousse::WallCollisionRecord<Type>::operator=
(
  const WallCollisionRecord<Type>& rhs
)
{
  // Check for assignment to self
  if (this == &rhs) {
    FATAL_ERROR_IN
    (
      "mousse::WallCollisionRecord<Type>::operator="
      "(const mousse::WallCollisionRecord<Type>&)"
    )
    << "Attempted assignment to self"
    << abort(FatalError);
  }
  accessed_ = rhs.accessed_;
  pRel_ = rhs.pRel_;
  data_ = rhs.data_;
}

//  IOStream operators
#include "_wall_collision_record_io.ipp"

