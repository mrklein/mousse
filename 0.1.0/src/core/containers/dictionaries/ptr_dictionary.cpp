// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptr_dictionary.hpp"
// Constructors 
template<class T>
mousse::PtrDictionary<T>::PtrDictionary(const label size)
:
  DictionaryBase<DLPtrList<T>, T>(size)
{}
template<class T>
mousse::PtrDictionary<T>::PtrDictionary(const PtrDictionary& dict)
:
  DictionaryBase<DLPtrList<T>, T>(dict)
{}
template<class T>
template<class INew>
mousse::PtrDictionary<T>::PtrDictionary(Istream& is, const INew& iNew)
:
  DictionaryBase<DLPtrList<T>, T>(is, iNew)
{}
template<class T>
mousse::PtrDictionary<T>::PtrDictionary(Istream& is)
:
  DictionaryBase<DLPtrList<T>, T>(is)
{}
