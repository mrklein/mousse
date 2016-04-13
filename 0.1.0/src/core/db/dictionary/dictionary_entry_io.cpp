// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "key_type.hpp"
#include "dictionary_entry.hpp"
#include "iostreams.hpp"


// Constructors 
mousse::dictionaryEntry::dictionaryEntry
(
  const dictionary& parentDict,
  Istream& is
)
:
  entry{keyType(is)},
  dictionary{parentDict, is}
{
  is.fatalCheck
  (
    "dictionaryEntry::dictionaryEntry"
    "(const dictionary& parentDict, Istream&)"
  );
}


mousse::dictionaryEntry::dictionaryEntry
(
  const keyType& key,
  const dictionary& parentDict,
  Istream& is
)
:
  entry{key},
  dictionary{key, parentDict, is}
{
  is.fatalCheck
  (
    "dictionaryEntry::dictionaryEntry"
    "(const keyType&, const dictionary& parentDict, Istream&)"
  );
}


// Member Functions 
void mousse::dictionaryEntry::write(Ostream& os) const
{
  // write keyword with indent but without trailing spaces
  os.indent();
  os.write(keyword());
  dictionary::write(os);
}


// Ostream operator 
mousse::Ostream& mousse::operator<<(Ostream& os, const dictionaryEntry& de)
{
  de.write(os);
  return os;
}


template<>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const InfoProxy<dictionaryEntry>& ip
)
{
  const dictionaryEntry& e = ip.t_;
  os << "    dictionaryEntry '" << e.keyword() << "'" << endl;
  return os;
}
