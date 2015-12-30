// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_processor_field_hpp_
#define io_processor_field_hpp_
#include "processor_field.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<processorField> IOprocessorField;
}
#endif
