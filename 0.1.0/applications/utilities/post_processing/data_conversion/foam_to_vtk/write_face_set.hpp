#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_FACE_SET_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_FACE_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_face_set.cpp
#include "vtk_mesh.hpp"
#include "face_set.hpp"
namespace mousse
{
// Write lagrangian fields.
void writeFaceSet
(
  const bool binary,
  const vtkMesh& vMesh,
  const faceSet& set,
  const fileName& fileName
);
}  // namespace mousse
#endif
