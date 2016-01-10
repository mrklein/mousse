// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uindirect_list.hpp"
#include "ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// Ostream Operator
template<class T>
mousse::Ostream& mousse::operator<<
(
  mousse::Ostream& os,
  const mousse::UIndirectList<T>& L
)
{
  // Write list contents depending on data format
  if (os.format() == IOstream::ASCII || !contiguous<T>())
  {
    bool uniform = false;
    if (L.size() > 1 && contiguous<T>())
    {
      uniform = true;
      FOR_ALL(L, i)
      {
        if (L[i] != L[0])
        {
          uniform = false;
          break;
        }
      }
    }
    if (uniform)
    {
      // Write size and start delimiter
      os << L.size() << token::BEGIN_BLOCK;
      // Write contents
      os << L[0];
      // Write end delimiter
      os << token::END_BLOCK;
    }
    else if (L.size() <= 1 || (L.size() < 11 && contiguous<T>()))
    {
      // Write size and start delimiter
      os << L.size() << token::BEGIN_LIST;
      // Write contents
      FOR_ALL(L, i)
      {
        if (i) os << token::SPACE;
        os << L[i];
      }
      // Write end delimiter
      os << token::END_LIST;
    }
    else
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
    }
  }
  else
  {
    // this is annoying, and wasteful, but there's currently no alternative
    os << nl << L.size() << nl;
    if (L.size())
    {
      List<T> lst = L();
      os.write
      (
        reinterpret_cast<const char*>(lst.cdata()),
        lst.byteSize()
      );
    }
  }
  // Check state of IOstream
  os.check("Ostream& operator<<(Ostream&, const UIndirectList&)");
  return os;
}
