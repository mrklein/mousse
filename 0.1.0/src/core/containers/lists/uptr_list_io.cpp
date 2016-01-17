// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uptr_list.hpp"
#include "ostream.hpp"

// Ostream Operators
template<class T>
mousse::Ostream& mousse::operator<<(Ostream& os, const UPtrList<T>& L)
{
  // Write size and start delimiter
  os << nl << L.size() << nl << token::BEGIN_LIST;
  // Write contents
  FOR_ALL(L, i)
  {
    os << nl << L[i];
  }
  // Write end delimiter
  os << nl << token::END_LIST << nl;
  // Check state of IOstream
  os.check("Ostream& operator<<(Ostream&, const UPtrList&)");
  return os;
}
