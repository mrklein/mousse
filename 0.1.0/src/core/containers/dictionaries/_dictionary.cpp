// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_dictionary.hpp"
// Constructors 
template<class T>
mousse::Dictionary<T>::Dictionary(const label size)
:
  DictionaryBase<IDLList<T>, T>(size)
{}
template<class T>
mousse::Dictionary<T>::Dictionary(const Dictionary& dict)
:
  DictionaryBase<IDLList<T>, T>(dict)
{}
// Member Functions 
template<class T>
bool mousse::Dictionary<T>::erase(const word& keyword)
{
  T* tPtr = this->remove(keyword);
  if (tPtr)
  {
    delete tPtr;
    return true;
  }
  else
  {
    return false;
  }
}
