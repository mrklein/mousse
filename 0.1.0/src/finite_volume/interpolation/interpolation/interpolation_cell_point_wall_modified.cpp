// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_cell_point_wall_modified.hpp"
// Constructor
template<class Type>
mousse::interpolationCellPointWallModified<Type>::
interpolationCellPointWallModified
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  interpolationCellPoint<Type>(psi)
{}
