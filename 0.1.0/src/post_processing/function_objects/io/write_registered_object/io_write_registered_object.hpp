// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_write_registered_object_hpp_
#define io_write_registered_object_hpp_
#include "write_registered_object.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<writeRegisteredObject> IOwriteRegisteredObject;
}
#endif
