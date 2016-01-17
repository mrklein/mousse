// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ostring_stream.hpp"
template<class Type>
void mousse::functionObjectFile::writeHeaderValue
(
  Ostream& os,
  const string& property,
  const Type& value
) const
{
  os  << setw(1) << '#' << setw(1) << ' '
    << setw(charWidth() - 2) << setf(ios_base::left) << property.c_str()
    << setw(1) << ':' << setw(1) << ' ' << value << nl;
}
