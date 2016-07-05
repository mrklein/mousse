#ifndef CORE_DB_CALLBACK_REGISTRY_CALLBACK_REGISTRY_HPP_
#define CORE_DB_CALLBACK_REGISTRY_CALLBACK_REGISTRY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CallbackRegistry
// Description
//   Base class with which callbacks are registered.
//   Derive from this class and extend by adding the appropriate callback
//   functions that loop and call the callback functions for each entry.

#include "uidl_list.hpp"
#include "class_name.hpp"


namespace mousse {

// Forward declaration of classes
template<class CallbackType>
class Callback;

TEMPLATE_NAME(CallbackRegistry);
template<class CallbackType>
class CallbackRegistry
:
  public CallbackRegistryName,
  public UIDLList<CallbackType>
{
public:
  // Constructors
    //- Construct null
    CallbackRegistry();

    //- Disallow default bitwise copy construct
    CallbackRegistry(const CallbackRegistry&) = delete;

    //- Disallow default bitwise assignment
    CallbackRegistry& operator=(const CallbackRegistry&) = delete;

  //- Destructor
  virtual ~CallbackRegistry();
};

}  // namespace mousse

#include "callback_registry.ipp"

#endif
