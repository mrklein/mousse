// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"
// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const DataEntry<Type>& de
)
{
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const DataEntry<Type>&)"
  );
  os  << de.name_;
  return os;
}
template<class Type>
void mousse::DataEntry<Type>::writeData(Ostream& os) const
{
  os.writeKeyword(name_) << type();
}
