#ifndef POST_PROCESSING_FUNCTION_OBJECTS_CLOUD_CLOUD_INFO_CLOUD_INFO_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_CLOUD_CLOUD_INFO_CLOUD_INFO_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cloud_info.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<cloudInfo>
  cloudInfoFunctionObject;

}

#endif

