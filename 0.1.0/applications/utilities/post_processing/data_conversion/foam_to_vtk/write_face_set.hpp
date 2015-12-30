// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_face_set.cpp
#ifndef write_face_set_hpp_
#define write_face_set_hpp_
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
