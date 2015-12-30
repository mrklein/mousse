// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   region_size_distribution_function_object.cpp
#ifndef region_size_distribution_function_object_hpp_
#define region_size_distribution_function_object_hpp_
#include "region_size_distribution.hpp"
#include "output_filter_function_object.hpp"
namespace mousse
{
  typedef OutputFilterFunctionObject<regionSizeDistribution>
    regionSizeDistributionFunctionObject;
}
#endif
