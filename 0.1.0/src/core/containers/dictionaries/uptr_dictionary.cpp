// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "uptr_dictionary.hpp"
// Constructors 
template<class T>
mousse::UPtrDictionary<T>::UPtrDictionary(const label size)
:
  DictionaryBase<DLList<T*>, T>(size)
{}
template<class T>
mousse::UPtrDictionary<T>::UPtrDictionary(const UPtrDictionary& dict)
:
  DictionaryBase<DLList<T*>, T>(dict)
{}
