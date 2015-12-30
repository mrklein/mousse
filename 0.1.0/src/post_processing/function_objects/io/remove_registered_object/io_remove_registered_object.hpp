// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_remove_registered_object_hpp_
#define io_remove_registered_object_hpp_
#include "remove_registered_object.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<removeRegisteredObject> IOremoveRegisteredObject;
}
#endif
