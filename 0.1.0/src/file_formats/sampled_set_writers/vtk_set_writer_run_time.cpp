// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_set_writer.hpp"
#include "writers.hpp"
#include "add_to_run_time_selection_table.hpp"
namespace mousse
{
  makeSetWriters(vtkSetWriter);
}
