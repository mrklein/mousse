// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_collision_record.hpp"
#include "iostreams.hpp"
// Constructors 
template<class Type>
mousse::WallCollisionRecord<Type>::WallCollisionRecord(Istream& is)
:
  accessed_(is),
  pRel_(is),
  data_(is)
{
  // Check state of Istream
  is.check
  (
    "mousse::WallCollisionRecord<Type>::WallCollisionRecord(mousse::Istream&)"
  );
}
// IOstream Operators 
template<class Type>
mousse::Istream& mousse::operator>>(Istream& is, WallCollisionRecord<Type>& wCR)
{
  is  >> wCR.accessed_ >> wCR.pRel_ >> wCR.data_;
  // Check state of Istream
  is.check
  (
    "mousse::Istream&"
    "mousse::operator>>(mousse::Istream&, mousse::WallCollisionRecord<Type>&)"
  );
  return is;
}
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const WallCollisionRecord<Type>& wCR
)
{
  os  << wCR.accessed_
    << token::SPACE << wCR.pRel_
    << token::SPACE << wCR.data_;
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::WallCollisionRecord<Type>&)"
  );
  return os;
}
