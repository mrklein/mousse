#ifndef UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_PATCH_HPP_
#define UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_PATCH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "primitive_face_patch.hpp"


namespace mousse {

// Write faceSet
void writePatch
(
  const bool binary,
  const word& setName,
  const primitiveFacePatch& fp,
  const word& fieldName,
  labelList& fieldValues,
  const fileName& fileName
);

}  // namespace mousse

#endif

