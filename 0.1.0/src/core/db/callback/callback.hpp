#ifndef CORE_DB_CALLBACK_CALLBACK_HPP_
#define CORE_DB_CALLBACK_CALLBACK_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Callback
// Description
//   Abstract class to handle automatic call-back registration with the
//   CallbackRegistry.  Derive from this class and extend by adding the
//   appropriate callback functions.
// SourceFiles
//   callback.cpp


#include "callback_registry.hpp"

namespace mousse
{

template<class CallbackType>
class Callback
:
  public CallbackRegistry<CallbackType>::link
{
  // Private data

    //- CallbackRegistry reference
    CallbackRegistry<CallbackType>& cbr_;

public:
  // Constructors

    //- Construct and register with CallbackRegistry
    Callback(CallbackRegistry<CallbackType>&);

    //- Construct as copy
    Callback(const Callback<CallbackType>&);

    //- Disallow default bitwise assignment
    Callback<CallbackType>& operator=(const Callback<CallbackType>&) = delete;

  //- Destructor
  virtual ~Callback();

  // Member functions

    // Registration

      //- Register Callback with CallbackRegistry
      bool checkIn();

      //- Check-out Callback from CallbackRegistry
      bool checkOut();

};

}  // namespace mousse

#ifdef NoRepository
#   include "callback.cpp"
#endif
#endif
