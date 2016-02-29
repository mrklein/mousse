#ifndef POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TURBULENCE_FIELDS_IO_TURBULENCE_FIELDS_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_UTILITIES_TURBULENCE_FIELDS_IO_TURBULENCE_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "turbulence_fields.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<turbulenceFields> IOturbulenceFields;
}
#endif
