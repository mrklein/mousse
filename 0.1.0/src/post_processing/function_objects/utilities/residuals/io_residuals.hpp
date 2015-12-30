// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_residuals_hpp_
#define io_residuals_hpp_
#include "residuals.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<residuals> IOresiduals;
}
#endif
