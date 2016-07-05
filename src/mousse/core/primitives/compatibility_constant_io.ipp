// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const CompatibilityConstant<Type>& cnst
)
{
  if (os.format() == IOstream::ASCII) {
    os << static_cast<const DataEntry<Type>& >(cnst)
      << token::SPACE << cnst.value_;
  } else {
    os << static_cast<const DataEntry<Type>& >(cnst);
    os.write
    (
      reinterpret_cast<const char*>(&cnst.value_),
      sizeof(cnst.value_)
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const CompatibilityConstant<Type>&)"
  );
  return os;
}


template<class Type>
void mousse::CompatibilityConstant<Type>::writeData(Ostream& os) const
{
  os.writeKeyword(this->name_) << value_ << token::END_STATEMENT << nl;
}
