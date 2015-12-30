// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_point_set.cpp
#ifndef write_point_set_hpp_
#define write_point_set_hpp_
#include "vtk_mesh.hpp"
#include "point_set.hpp"
namespace mousse
{
// Write lagrangian fields.
void writePointSet
(
  const bool binary,
  const vtkMesh& vMesh,
  const pointSet& set,
  const fileName& fileName
);
}  // namespace mousse
#endif
