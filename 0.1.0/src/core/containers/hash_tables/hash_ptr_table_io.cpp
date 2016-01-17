// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "hash_ptr_table.hpp"
#include "istream.hpp"
#include "ostream.hpp"
#include "inew.hpp"
#include "dictionary.hpp"

// Private Member Functions 
template<class T, class Key, class Hash>
template<class INew>
void mousse::HashPtrTable<T, Key, Hash>::read(Istream& is, const INew& inewt)
{
  is.fatalCheck("HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)");
  token firstToken{is};
  is.fatalCheck
  (
    "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&) : "
    "reading first token"
  );
  if (firstToken.isLabel())
  {
    label s = firstToken.labelToken();
    // Read beginning of contents
    char delimiter = is.readBeginList("HashPtrTable<T, Key, Hash>");
    if (s)
    {
      if (2*s > this->tableSize_)
      {
        this->resize(2*s);
      }
      if (delimiter == token::BEGIN_LIST)
      {
        for (label i=0; i<s; i++)
        {
          Key key;
          is >> key;
          this->insert(key, inewt(key, is).ptr());
          is.fatalCheck
          (
            "HashPtrTable<T, Key, Hash>::"
            "read(Istream&, const INew&) : reading entry"
          );
        }
      }
      else
      {
        FATAL_IO_ERROR_IN
        (
          "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)",
          is
        )
        << "incorrect first token, '(', found " << firstToken.info()
        << exit(FatalIOError);
      }
    }
    // Read end of contents
    is.readEndList("HashPtrTable");
  }
  else if (firstToken.isPunctuation())
  {
    if (firstToken.pToken() != token::BEGIN_LIST)
    {
      FATAL_IO_ERROR_IN
      (
        "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)",
        is
      )
      << "incorrect first token, '(', found " << firstToken.info()
      << exit(FatalIOError);
    }
    token lastToken{is};
    while (!(lastToken.isPunctuation()
             && lastToken.pToken() == token::END_LIST))
    {
      is.putBack(lastToken);
      Key key;
      is >> key;
      this->insert(key, inewt(key, is).ptr());
      is.fatalCheck
      (
        "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&) : "
        "reading entry"
      );
      is >> lastToken;
    }
  }
  else
  {
    FATAL_IO_ERROR_IN
    (
      "HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)",
      is
    )   << "incorrect first token, expected <int> or '(', found "
      << firstToken.info()
      << exit(FatalIOError);
  }
  is.fatalCheck("HashPtrTable<T, Key, Hash>::read(Istream&, const INew&)");
}


template<class T, class Key, class Hash>
template<class INew>
void mousse::HashPtrTable<T, Key, Hash>::read
(
  const dictionary& dict,
  const INew& inewt
)
{
  FOR_ALL_CONST_ITER(dictionary, dict, iter)
  {
    this->insert
    (
      iter().keyword(),
      inewt(dict.subDict(iter().keyword())).ptr()
    );
  }
}


template<class T, class Key, class Hash>
void mousse::HashPtrTable<T, Key, Hash>::write(Ostream& os) const
{
  for
  (
    typename HashPtrTable<T, Key, Hash>::const_iterator
    iter = this->begin();
    iter != this->end();
    ++iter
  )
  {
    const T* ptr = iter();
    ptr->write(os);
  }
}


// Constructors 
template<class T, class Key, class Hash>
template<class INew>
mousse::HashPtrTable<T, Key, Hash>::HashPtrTable(Istream& is, const INew& inewt)
{
  this->read(is, inewt);
}


template<class T, class Key, class Hash>
mousse::HashPtrTable<T, Key, Hash>::HashPtrTable(Istream& is)
{
  this->read(is, INew<T>());
}


template<class T, class Key, class Hash>
mousse::HashPtrTable<T, Key, Hash>::HashPtrTable(const dictionary& dict)
{
  this->read(dict, INew<T>());
}


// IOstream Operators 
template<class T, class Key, class Hash>
mousse::Istream& mousse::operator>>(Istream& is, HashPtrTable<T, Key, Hash>& L)
{
  L.clear();
  L.read(is, INew<T>());
  return is;
}


template<class T, class Key, class Hash>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const HashPtrTable<T, Key, Hash>& L
)
{
  // Write size and start delimiter
  os << nl << L.size() << nl << token::BEGIN_LIST << nl;
  // Write contents
  for
  (
    typename HashPtrTable<T, Key, Hash>::const_iterator iter = L.begin();
    iter != L.end();
    ++iter
  )
  {
    os << iter.key() << token::SPACE << *iter() << nl;
  }
  // Write end delimiter
  os << token::END_LIST;
  // Check state of IOstream
  os.check("Ostream& operator<<(Ostream&, const HashPtrTable&)");
  return os;
}
