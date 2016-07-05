#ifndef UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_POINT_SET_HPP_
#define UTILITIES_MESH_MANIPULATION_SET_SET_WRITE_POINT_SET_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_mesh.hpp"
#include "point_set.hpp"


namespace mousse {

//- Write pointSet to vtk polydata file.
//  Only one data which is original pointID.
void writePointSet
(
  const bool binary,
  const primitiveMesh& mesh,
  const topoSet& set,
  const fileName& fileName
);

}  // namespace mousse

#endif

