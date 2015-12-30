// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "buffered_accumulator.hpp"
#include "iostreams.hpp"
// IOstream Operators 
template<class Type>
mousse::Ostream&
mousse::operator<<(Ostream& os, const bufferedAccumulator<Type>& bA)
{
  os  << bA.averagesTaken_
    << static_cast<const List< Field<Type> >&>(bA)
    << bA.bufferOffsets();
  // Check state of Ostream
  os.check
  (
    "mousse::Ostream& mousse::operator<<(mousse::Ostream&, "
    "const mousse::bufferedAccumulator&)"
  );
  return os;
}
