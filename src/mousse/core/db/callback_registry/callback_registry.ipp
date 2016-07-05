// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "callback_registry.hpp"


// Constructors 
template<class CallbackType>
mousse::CallbackRegistry<CallbackType>::CallbackRegistry()
:
  CallbackRegistryName{},
  UIDLList<CallbackType>{}
{}


// Destructor 
template<class CallbackType>
mousse::CallbackRegistry<CallbackType>::~CallbackRegistry()
{
  FOR_ALL_ITER(typename CallbackRegistry<CallbackType>, *this, iter) {
    iter().Callback<CallbackType>::checkOut();
  }
}
