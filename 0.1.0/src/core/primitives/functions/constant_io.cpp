// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant.hpp"
// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const Constant<Type>& cnst
)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const DataEntry<Type>& >(cnst)
      << token::SPACE << cnst.value_;
  }
  else
  {
    os  << static_cast<const DataEntry<Type>& >(cnst);
    os.write
    (
      reinterpret_cast<const char*>(&cnst.value_),
      sizeof(cnst.value_)
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const Constant<Type>&)"
  );
  return os;
}
template<class Type>
void mousse::Constant<Type>::writeData(Ostream& os) const
{
  DataEntry<Type>::writeData(os);
  os  << token::SPACE << value_ << token::END_STATEMENT << nl;
}
