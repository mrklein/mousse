// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_force_coeffs_hpp_
#define io_force_coeffs_hpp_
#include "force_coeffs.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<forceCoeffs> IOforceCoeffs;
}
#endif
