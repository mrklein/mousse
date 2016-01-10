// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_entry.hpp"
#include "dictionary.hpp"
// Constructors 
template<class T>
mousse::primitiveEntry::primitiveEntry(const keyType& key, const T& t)
:
  entry(key),
  ITstream(key, tokenList(10))
{
  OStringStream os;
  os  << t << token::END_STATEMENT;
  readEntry(dictionary::null, IStringStream(os.str())());
}
