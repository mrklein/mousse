#ifndef UTILITIES_MESH_CONVERSION_FLUENT_MESH_TO_FOAM_CELL_SHAPE_RECOGNITION_HPP_
#define UTILITIES_MESH_CONVERSION_FLUENT_MESH_TO_FOAM_CELL_SHAPE_RECOGNITION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_shape.hpp"
#include "cell_modeller.hpp"
#include "face_list.hpp"
#include "ptr_list.hpp"
namespace mousse
{
cellShape extrudedTriangleCellShape
(
  const label cellIndex,
  const labelList& faceLabels,
  const faceList& faces,
  const labelList& owner,
  const labelList& neighbour,
  const label pointOffset,
  faceList& frontAndBackFaces
);
cellShape extrudedQuadCellShape
(
  const label cellIndex,
  const labelList& faceLabels,
  const faceList& faces,
  const labelList& owner,
  const labelList& neighbour,
  const label pointOffset,
  faceList& frontAndBackFaces
);
cellShape create3DCellShape
(
  const label cellIndex,
  const labelList& faceLabels,
  const faceList& faces,
  const labelList& owner,
  const labelList& neighbour,
  const label fluentCellModelID
);
}  // namespace mousse
#endif
