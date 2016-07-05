// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "multivariate_selection_scheme.hpp"
#include "limited_surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "upwind.hpp"


template<class Type>
mousse::multivariateSelectionScheme<Type>::multivariateSelectionScheme
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
  faceFlux_{faceFlux},
  weights_
  {
    {
      "multivariateWeights",
      mesh.time().timeName(),
      mesh
    },
    mesh,
    dimless
  }
{
  typename multivariateSurfaceInterpolationScheme<Type>::
    fieldTable::const_iterator iter = this->fields().begin();
  surfaceScalarField limiter
  {
    limitedSurfaceInterpolationScheme<Type>::New
    (
      mesh,
      faceFlux_,
      schemes_.lookup(iter()->name())
    )().limiter(*iter())
  };
  for (++iter; iter != this->fields().end(); ++iter) {
    limiter = min
    (
      limiter,
      limitedSurfaceInterpolationScheme<Type>::New
      (
        mesh,
        faceFlux_,
        schemes_.lookup(iter()->name())
      )().limiter(*iter())
    );
  }
  weights_ = limiter*mesh.surfaceInterpolation::weights()
    + (scalar(1) - limiter)*upwind<Type>(mesh, faceFlux_).weights();
}
