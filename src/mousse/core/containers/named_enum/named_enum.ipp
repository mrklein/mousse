// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "named_enum.hpp"


// Constructors 
template<class Enum, int nEnum>
mousse::NamedEnum<Enum, nEnum>::NamedEnum()
:
  HashTable<int>{2*nEnum}
{
  for (int enumI = 0; enumI < nEnum; ++enumI) {
    if (!names[enumI] || names[enumI][0] == '\0') {
      stringList goodNames{enumI};
      for (int i = 0; i < enumI; ++i) {
        goodNames[i] = names[i];
      }
      FATAL_ERROR_IN("NamedEnum<Enum, nEnum>::NamedEnum()")
        << "Illegal enumeration name at position " << enumI << endl
        << "after entries " << goodNames << ".\n"
        << "Possibly your NamedEnum<Enum, nEnum>::names array"
        << " is not of size " << nEnum << endl
        << abort(FatalError);
    }
    insert(names[enumI], enumI);
  }
}


// Member Functions 
template<class Enum, int nEnum>
Enum mousse::NamedEnum<Enum, nEnum>::read(Istream& is) const
{
  const word name(is);
  HashTable<int>::const_iterator iter = find(name);
  if (iter == HashTable<int>::end()) {
    FATAL_IO_ERROR_IN
    (
      "NamedEnum<Enum, nEnum>::read(Istream&) const", is
    )
    << name << " is not in enumeration: "
    << sortedToc() << exit(FatalIOError);
  }
  return Enum(iter());
}


template<class Enum, int nEnum>
void mousse::NamedEnum<Enum, nEnum>::write(const Enum e, Ostream& os) const
{
  os << operator[](e);
}


template<class Enum, int nEnum>
mousse::stringList mousse::NamedEnum<Enum, nEnum>::strings()
{
  stringList lst{nEnum};
  label nElem = 0;
  for (int enumI = 0; enumI < nEnum; ++enumI) {
    if (names[enumI] && names[enumI][0]) {
      lst[nElem++] = names[enumI];
    }
  }
  lst.setSize(nElem);
  return lst;
}


template<class Enum, int nEnum>
mousse::wordList mousse::NamedEnum<Enum, nEnum>::words()
{
  wordList lst{nEnum};
  label nElem = 0;
  for (int enumI = 0; enumI < nEnum; ++enumI) {
    if (names[enumI] && names[enumI][0]) {
      lst[nElem++] = names[enumI];
    }
  }
  lst.setSize(nElem);
  return lst;
}
