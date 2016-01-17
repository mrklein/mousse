// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fixed_list.hpp"
#include "istream.hpp"
#include "ostream.hpp"
#include "token.hpp"
#include "contiguous.hpp"

// IOstream Operators 
template<class T, unsigned Size>
mousse::FixedList<T, Size>::FixedList(Istream& is)
{
  operator>>(is, *this);
}


template<class T, unsigned Size>
mousse::Istream& mousse::operator>>(mousse::Istream& is, FixedList<T, Size>& L)
{
  is.fatalCheck("operator>>(Istream&, FixedList<T, Size>&)");
  if (is.format() == IOstream::ASCII || !contiguous<T>())
  {
    token firstToken(is);
    is.fatalCheck
    (
      "operator>>(Istream&, FixedList<T, Size>&) : reading first token"
    );
    if (firstToken.isCompound())
    {
      L = dynamicCast<token::Compound<List<T> > >
      (
        firstToken.transferCompoundToken(is)
      );
    }
    else if (firstToken.isLabel())
    {
      label s = firstToken.labelToken();
      // Set list length to that read
      L.checkSize(s);
    }
    else if (!firstToken.isPunctuation())
    {
      FATAL_IO_ERROR_IN("operator>>(Istream&, FixedList<T, Size>&)", is)
        << "incorrect first token, expected <label> "
         "or '(' or '{', found "
        << firstToken.info()
        << exit(FatalIOError);
    }
    else
    {
      // Putback the opening bracket
      is.putBack(firstToken);
    }
    // Read beginning of contents
    char delimiter = is.readBeginList("FixedList");
    if (delimiter == token::BEGIN_LIST)
    {
      for (unsigned i=0; i<Size; i++)
      {
        is >> L[i];
        is.fatalCheck
        (
          "operator>>(Istream&, FixedList<T, Size>&) : "
          "reading entry"
        );
      }
    }
    else
    {
      T element;
      is >> element;
      is.fatalCheck
      (
        "operator>>(Istream&, FixedList<T, Size>&) : "
        "reading the single entry"
      );
      for (unsigned i=0; i<Size; i++)
      {
        L[i] = element;
      }
    }
    // Read end of contents
    is.readEndList("FixedList");
  }
  else
  {
    is.read(reinterpret_cast<char*>(L.data()), Size*sizeof(T));
    is.fatalCheck
    (
      "operator>>(Istream&, FixedList<T, Size>&) : "
      "reading the binary block"
    );
  }
  return is;
}


// Ostream Operator
template<class T, unsigned Size>
void mousse::FixedList<T, Size>::writeEntry(Ostream& os) const
{
  if
  (
    size()
  && token::compound::isCompound
    (
      "List<" + word(pTraits<T>::typeName) + '>'
    )
  )
  {
    os  << word("List<" + word(pTraits<T>::typeName) + '>') << " ";
  }
  os << *this;
}


template<class T, unsigned Size>
void mousse::FixedList<T, Size>::writeEntry
(
  const word& keyword,
  Ostream& os
) const
{
  os.writeKeyword(keyword);
  writeEntry(os);
  os << token::END_STATEMENT << endl;
}


template<class T, unsigned Size>
mousse::Ostream& mousse::operator<<(Ostream& os, const FixedList<T, Size>& L)
{
  // Write list contents depending on data format
  if (os.format() == IOstream::ASCII || !contiguous<T>())
  {
    bool uniform = false;
    if (Size > 1 && contiguous<T>())
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
      // Write size (so it is valid dictionary entry) and start delimiter
      os << L.size() << token::BEGIN_BLOCK;
      // Write contents
      os << L[0];
      // Write end delimiter
      os << token::END_BLOCK;
    }
    else if (Size <= 1 ||(Size < 11 && contiguous<T>()))
    {
      // Write start delimiter
      os << token::BEGIN_LIST;
      // Write contents
      FOR_ALL(L, i)
      {
        if (i > 0) os << token::SPACE;
        os << L[i];
      }
      // Write end delimiter
      os << token::END_LIST;
    }
    else
    {
      // Write start delimiter
      os << nl << token::BEGIN_LIST;
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
    os.write(reinterpret_cast<const char*>(L.cdata()), Size*sizeof(T));
  }
  // Check state of IOstream
  os.check("Ostream& operator<<(Ostream&, const FixedList&)");
  return os;
}
