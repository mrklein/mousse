// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial_entry.hpp"
// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const PolynomialEntry<Type>& poly
)
{
  if (os.format() == IOstream::ASCII)
  {
    os  << static_cast<const DataEntry<Type>& >(poly)
      << token::SPACE << poly.coeffs_;
  }
  else
  {
    os  << static_cast<const DataEntry<Type>& >(poly);
    os.write
    (
      reinterpret_cast<const char*>(&poly.coeffs_),
      sizeof(poly.coeffs_)
    );
  }
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const PolynomialEntry&)"
  );
  return os;
}
template<class Type>
void mousse::PolynomialEntry<Type>::writeData(Ostream& os) const
{
  DataEntry<Type>::writeData(os);
  os  << nl << indent << coeffs_ << token::END_STATEMENT << nl;
}
