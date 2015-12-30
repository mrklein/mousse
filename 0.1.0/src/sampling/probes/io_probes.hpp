// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef io_probes_hpp_
#define io_probes_hpp_
#include "probes.hpp"
#include "patch_probes.hpp"
#include "io_output_filter.hpp"
namespace mousse
{
  typedef IOOutputFilter<probes> IOprobes;
  typedef IOOutputFilter<patchProbes> IOpatchProbes;
}
#endif
