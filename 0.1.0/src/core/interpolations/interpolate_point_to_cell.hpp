// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   interpolate_point_to_cell.cpp

#ifndef interpolate_point_to_cell_hpp_
#define interpolate_point_to_cell_hpp_

#include "geometric_field.hpp"
#include "point_mesh.hpp"

namespace mousse
{

// Interpolate field to cell center.
template<class Type>
Type interpolatePointToCell
(
  const GeometricField<Type, pointPatchField, pointMesh>& ptf,
  const label celli
);

}  // namespace mousse
#ifdef NoRepository
#   include "interpolate_point_to_cell.cpp"
#endif
#endif
