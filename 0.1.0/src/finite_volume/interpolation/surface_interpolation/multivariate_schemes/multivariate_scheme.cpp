// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "upwind.hpp"
template<class Type, class Scheme>
mousse::multivariateScheme<Type, Scheme>::multivariateScheme
(
  const fvMesh& mesh,
  const typename multivariateSurfaceInterpolationScheme<Type>::
    fieldTable& fields,
  const surfaceScalarField& faceFlux,
  Istream& schemeData
)
:
  multivariateSurfaceInterpolationScheme<Type>
  (
    mesh,
    fields,
    faceFlux,
    schemeData
  ),
  Scheme::LimiterType(schemeData),
  faceFlux_(faceFlux),
  weights_
  (
    IOobject
    (
      "multivariateWeights",
      mesh.time().timeName(),
      mesh
    ),
    mesh,
    dimless
  )
{
  typename multivariateSurfaceInterpolationScheme<Type>::
    fieldTable::const_iterator iter = this->fields().begin();
  surfaceScalarField limiter
  (
    Scheme(mesh, faceFlux_, *this).limiter(*iter())
  );
  for (++iter; iter != this->fields().end(); ++iter)
  {
    limiter = min
    (
      limiter,
      Scheme(mesh, faceFlux_, *this).limiter(*iter())
    );
  }
  weights_ =
    limiter*mesh.surfaceInterpolation::weights()
   + (scalar(1) - limiter)*upwind<Type>(mesh, faceFlux_).weights();
}
