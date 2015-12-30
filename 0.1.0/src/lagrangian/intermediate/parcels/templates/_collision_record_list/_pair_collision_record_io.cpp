// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_pair_collision_record.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Type>
mousse::PairCollisionRecord<Type>::PairCollisionRecord(Istream& is)
:
  origProcOfOther_(readLabel(is)),
  origIdOfOther_(readLabel(is)),
  data_(is)
{
  // Check state of Istream
  is.check
  (
    "mousse::PairCollisionRecord<Type>::PairCollisionRecord(mousse::Istream&)"
  );
}
// IOstream Operators 
template<class Type>
mousse::Istream& mousse::operator>>(Istream& is, PairCollisionRecord<Type>& pCR)
{
  is  >> pCR.origProcOfOther_ >> pCR.origIdOfOther_ >> pCR.data_;
  // Check state of Istream
  is.check
  (
    "mousse::Istream&"
    "mousse::operator>>(mousse::Istream&, mousse::PairCollisionRecord<Type>&)"
  );
  return is;
}
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const PairCollisionRecord<Type>& pCR
)
{
  os  << pCR.origProcOfOther_
    << token::SPACE << pCR.origIdOfOther_
    << token::SPACE << pCR.data_;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::PairCollisionRecord<Type>&)"
  );
  return os;
}
