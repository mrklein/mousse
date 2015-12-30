// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_dsmc_fields_hpp_
#define io_dsmc_fields_hpp_
#include "dsmc_fields.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<dsmcFields> IOdsmcFields;
}
#endif
