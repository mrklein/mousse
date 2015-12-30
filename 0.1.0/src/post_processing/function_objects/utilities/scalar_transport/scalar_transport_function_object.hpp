// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   scalar_transport_function_object.cpp
#ifndef scalar_transport_function_object_hpp_
#define scalar_transport_function_object_hpp_
#include "scalar_transport.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<scalarTransport>
    scalarTransportFunctionObject;
}
#endif
