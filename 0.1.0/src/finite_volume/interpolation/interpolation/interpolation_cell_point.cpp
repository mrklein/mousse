// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_cell_point.hpp"
#include "vol_point_interpolation.hpp"
// Constructor
template<class Type>
mousse::interpolationCellPoint<Type>::interpolationCellPoint
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  interpolation<Type>(psi),
  psip_
  (
    volPointInterpolation::New(psi.mesh()).interpolate
    (
      psi,
      "volPointInterpolate(" + psi.name() + ')',
      true        // use cache
    )
  )
{
  // Uses cellPointWeight to do interpolation which needs tet decomposition
  (void)psi.mesh().tetBasePtIs();
}
