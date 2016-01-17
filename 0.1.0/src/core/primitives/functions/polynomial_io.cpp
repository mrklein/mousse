// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial.hpp"
// IOstream Operators 
template<int PolySize>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const Polynomial<PolySize>& poly
)
{
  os  << static_cast
      <VectorSpace<Polynomial<PolySize>, scalar, PolySize> >(poly);
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const Polynomial<PolySize>&)"
  );
  return os;
}
