#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TIME_ACTIVATED_FILE_UPDATE_TIME_ACTIVATED_FILE_UPDATE_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TIME_ACTIVATED_FILE_UPDATE_TIME_ACTIVATED_FILE_UPDATE_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_activated_file_update.hpp"
#include "output_filter_function_object.hpp"


namespace mousse {

typedef OutputFilterFunctionObject<timeActivatedFileUpdate>
  timeActivatedFileUpdateFunctionObject;

}

#endif

