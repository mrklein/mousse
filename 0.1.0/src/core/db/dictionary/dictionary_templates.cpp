// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary.hpp"
#include "primitive_entry.hpp"

// Member Functions 
template<class T>
T mousse::dictionary::lookupOrDefault
(
  const word& keyword,
  const T& deflt,
  bool recursive,
  bool patternMatch
) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);
  if (entryPtr)
  {
    return pTraits<T>(entryPtr->stream());
  }
  else
  {
    if (writeOptionalEntries)
    {
      IO_INFO_IN("dictionary::lookupOrDefault", *this)
        << "Optional entry '" << keyword << "' is not present,"
        << " returning the default value '" << deflt << "'"
        << endl;
    }
    return deflt;
  }
}


template<class T>
T mousse::dictionary::lookupOrAddDefault
(
  const word& keyword,
  const T& deflt,
  bool recursive,
  bool patternMatch
)
{
  const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);
  if (entryPtr)
  {
    return pTraits<T>(entryPtr->stream());
  }
  else
  {
    if (writeOptionalEntries)
    {
      IO_INFO_IN("dictionary::lookupOrAddDefault", *this)
        << "Optional entry '" << keyword << "' is not present,"
        << " adding and returning the default value '" << deflt << "'"
        << endl;
    }
    add(new primitiveEntry(keyword, deflt));
    return deflt;
  }
}


template<class T>
bool mousse::dictionary::readIfPresent
(
  const word& keyword,
  T& val,
  bool recursive,
  bool patternMatch
) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);
  if (entryPtr)
  {
    entryPtr->stream() >> val;
    return true;
  }
  else
  {
    if (writeOptionalEntries)
    {
      IO_INFO_IN("dictionary::readIfPresent", *this)
        << "Optional entry '" << keyword << "' is not present,"
        << " the default value '" << val << "' will be used."
        << endl;
    }
    return false;
  }
}


template<class T>
void mousse::dictionary::add(const keyType& k, const T& t, bool overwrite)
{
  add(new primitiveEntry(k, t), overwrite);
}


template<class T>
void mousse::dictionary::set(const keyType& k, const T& t)
{
  set(new primitiveEntry(k, t));
}
