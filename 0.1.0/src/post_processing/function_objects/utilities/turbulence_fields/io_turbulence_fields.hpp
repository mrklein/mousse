// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_turbulence_fields_hpp_
#define io_turbulence_fields_hpp_
#include "turbulence_fields.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<turbulenceFields> IOturbulenceFields;
}
#endif
