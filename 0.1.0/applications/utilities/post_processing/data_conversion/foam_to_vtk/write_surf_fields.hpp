#ifndef UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_SURF_FIELDS_HPP_
#define UTILITIES_POST_PROCESSING_DATA_CONVERSION_FOAM_TO_VTK_WRITE_SURF_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   write_surf_fields.cpp
#include "vtk_mesh.hpp"
#include "surface_mesh.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
// Write surface vector fields
void writeSurfFields
(
  const bool binary,
  const vtkMesh& vMesh,
  const fileName& fileName,
  const PtrList<surfaceVectorField>& surfVectorFields
);
}  // namespace mousse
#endif
