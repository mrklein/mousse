#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_POINT_SET_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_POINT_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vtk_mesh.hpp"
#include "point_set.hpp"


namespace mousse {

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

