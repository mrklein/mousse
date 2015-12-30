// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_wall_shear_stress_hpp_
#define io_wall_shear_stress_hpp_
#include "wall_shear_stress.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<wallShearStress> IOwallShearStress;
}
#endif
