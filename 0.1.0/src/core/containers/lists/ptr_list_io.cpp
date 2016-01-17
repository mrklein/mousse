// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptr_list.hpp"
#include "sl_list.hpp"
#include "istream.hpp"
#include "ostream.hpp"
#include "inew.hpp"

// Private Member Functions 
template<class T>
template<class INew>
void mousse::PtrList<T>::read(Istream& is, const INew& inewt)
{
  is.fatalCheck("PtrList<T>::read(Istream&, const INew&)");
  token firstToken{is};
  is.fatalCheck
  (
    "PtrList<T>::read(Istream&, const INew&) : "
    "reading first token"
  );
  if (firstToken.isLabel())
  {
    // Read size of list
    label s = firstToken.labelToken();
    setSize(s);
    // Read beginning of contents
    char delimiter = is.readBeginList("PtrList");
    if (s)
    {
      if (delimiter == token::BEGIN_LIST)
      {
        FOR_ALL(*this, i)
        {
          set(i, inewt(is));
          is.fatalCheck
          (
            "PtrList<T>::read(Istream&, const INew&) : "
            "reading entry"
          );
        }
      }
      else
      {
        T* tPtr = inewt(is).ptr();
        set(0, tPtr);
        is.fatalCheck
        (
          "PtrList<T>::read(Istream&, const INew&) : "
          "reading the single entry"
        );
        for (label i=1; i<s; i++)
        {
          set(i, tPtr->clone());
        }
      }
    }
    // Read end of contents
    is.readEndList("PtrList");
  }
  else if (firstToken.isPunctuation())
  {
    if (firstToken.pToken() != token::BEGIN_LIST)
    {
      FATAL_IO_ERROR_IN
      (
        "PtrList<T>::read(Istream&, const INew&)",
        is
      )   << "incorrect first token, '(', found " << firstToken.info()
        << exit(FatalIOError);
    }
    SLList<T*> sllPtrs;
    token lastToken{is};
    while(!(lastToken.isPunctuation()
            && lastToken.pToken() == token::END_LIST))
    {
      is.putBack(lastToken);
      if (is.eof())
      {
        FATAL_IO_ERROR_IN
        (
          "PtrList<T>::read(Istream&, const INew&)",
          is
        )
        << "Premature EOF after reading " << lastToken.info()
        << exit(FatalIOError);
      }
      sllPtrs.append(inewt(is).ptr());
      is >> lastToken;
    }
    setSize(sllPtrs.size());
    label i = 0;
    for
    (
      typename SLList<T*>::iterator iter = sllPtrs.begin();
      iter != sllPtrs.end();
      ++iter
    )
    {
      set(i++, iter());
    }
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "PtrList<T>::read(Istream&, const INew&)",
      is
    )
    << "incorrect first token, expected <int> or '(', found "
    << firstToken.info()
    << exit(FatalIOError);
  }
}


// Constructors 
template<class T>
template<class INew>
mousse::PtrList<T>::PtrList(Istream& is, const INew& inewt)
{
  read(is, inewt);
}


template<class T>
mousse::PtrList<T>::PtrList(Istream& is)
{
  read(is, INew<T>());
}


// Istream Operator 
template<class T>
mousse::Istream& mousse::operator>>(Istream& is, PtrList<T>& L)
{
  L.clear();
  L.read(is, INew<T>());
  return is;
}


// Ostream Operators
template<class T>
mousse::Ostream& mousse::operator<<(Ostream& os, const PtrList<T>& L)
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
  os.check("Ostream& operator<<(Ostream&, const PtrList&)");
  return os;
}
