// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "udictionary.hpp"
// Constructors 
template<class T>
mousse::UDictionary<T>::UDictionary()
{}
template<class T>
mousse::UDictionary<T>::UDictionary(const UDictionary& dict)
:
  DictionaryBase<UIDLList<T>, T>(dict)
{}
