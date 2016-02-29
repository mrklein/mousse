#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SCALAR_TRANSPORT_SCALAR_TRANSPORT_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_SCALAR_TRANSPORT_SCALAR_TRANSPORT_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   scalar_transport_function_object.cpp
#include "scalar_transport.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<scalarTransport>
    scalarTransportFunctionObject;
}
#endif
