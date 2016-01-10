// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary_base.hpp"
#include "iostreams.hpp"
// Ostream Operator 
template<class IDLListType, class T>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const DictionaryBase<IDLListType, T>& dict)
{
  for
  (
    typename IDLListType::const_iterator iter = dict.begin();
    iter != dict.end();
    ++iter
  )
  {
    os << *iter;
    // Check stream before going to next entry.
    if (!os.good())
    {
      Info
        << "operator<<(Ostream&, const DictionaryBase&) : "
        << "Can't write entry for DictionaryBase"
        << endl;
      return os;
    }
  }
  return os;
}
