// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_independent_scheme.hpp"
#include "limited_surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "upwind.hpp"


template<class Type>
mousse::multivariateIndependentScheme<Type>::multivariateIndependentScheme
(
  const fvMesh& mesh,
  const typename multivariateSurfaceInterpolationScheme<Type>::
    fieldTable& fields,
  const surfaceScalarField& faceFlux,
  Istream& schemeData
)
:
  multivariateSurfaceInterpolationScheme<Type>
  {
    mesh,
    fields,
    faceFlux,
    schemeData
  },
  schemes_{schemeData},
  faceFlux_{faceFlux}
{}

