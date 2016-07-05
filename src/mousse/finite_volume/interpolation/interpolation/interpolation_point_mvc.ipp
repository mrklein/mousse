// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "interpolation_point_mvc.hpp"
#include "vol_point_interpolation.hpp"


// Constructor
template<class Type>
mousse::interpolationPointMVC<Type>::interpolationPointMVC
(
  const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
  interpolation<Type>{psi},
  psip_
  {
    volPointInterpolation::New(psi.mesh()).interpolate
    (
      psi,
      "volPointInterpolate(" + psi.name() + ')',
      true  // use cache
    )
  }
{}
