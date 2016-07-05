// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_curl.hpp"
#include "fvc_grad.hpp"
#include "fv_mesh.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
curl
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  word nameCurlVf = "curl(" + vf.name() + ')';
  // Gausses theorem curl
  // tmp<GeometricField<Type, fvPatchField, volMesh>> tcurlVf =
  //     fvc::surfaceIntegrate(vf.mesh().Sf() ^ fvc::interpolate(vf));
  // Calculate curl as the Hodge dual of the skew-symmetric part of grad
  tmp<GeometricField<Type, fvPatchField, volMesh>> tcurlVf =
    2.0*(*skew(fvc::grad(vf, nameCurlVf)));
  tcurlVf().rename(nameCurlVf);
  return tcurlVf;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
curl
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvf
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh>> Curl{fvc::curl(tvf())};
  tvf.clear();
  return Curl;
}

}  // namespace fvc
}  // namespace mousse
