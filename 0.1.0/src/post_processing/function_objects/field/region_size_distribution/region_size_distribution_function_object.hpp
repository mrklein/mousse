#ifndef POST_PROCESSING_FUNCTION_OBJECTS_FIELD_REGION_SIZE_DISTRIBUTION_REGION_SIZE_DISTRIBUTION_FUNCTION_OBJECT_HPP_
#define POST_PROCESSING_FUNCTION_OBJECTS_FIELD_REGION_SIZE_DISTRIBUTION_REGION_SIZE_DISTRIBUTION_FUNCTION_OBJECT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   region_size_distribution_function_object.cpp
#include "region_size_distribution.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<regionSizeDistribution>
    regionSizeDistributionFunctionObject;
}
#endif
