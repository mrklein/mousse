// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary_entry.hpp"


// Constructors 
mousse::dictionaryEntry::dictionaryEntry
(
  const keyType& key,
  const dictionary& parentDict,
  const dictionary& dict
)
:
  entry{key},
  dictionary{parentDict, dict}
{}


mousse::dictionaryEntry::dictionaryEntry
(
  const dictionary& parentDict,
  const dictionaryEntry& dictEnt
)
:
  entry{dictEnt},
  dictionary{parentDict, dictEnt}
{}


// Member Functions 
mousse::label mousse::dictionaryEntry::startLineNumber() const
{
  if (size()) {
    return first()->startLineNumber();
  } else {
    return -1;
  }
}


mousse::label mousse::dictionaryEntry::endLineNumber() const
{
  if (size()) {
    return last()->endLineNumber();
  } else {
    return -1;
  }
}


mousse::ITstream& mousse::dictionaryEntry::stream() const
{
  FATAL_IO_ERROR_IN("ITstream& primitiveEntry::stream() const", *this)
    << "Attempt to return dictionary entry as a primitive"
    << abort(FatalIOError);
  return lookup("");
}


const mousse::dictionary& mousse::dictionaryEntry::dict() const
{
  return *this;
}


mousse::dictionary& mousse::dictionaryEntry::dict()
{
  return *this;
}

