// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_sn_grad.hpp"
#include "fv_mesh.hpp"
#include "sn_grad_scheme.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const word& name
)
{
  return fv::snGradScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().snGradScheme(name)
  )().snGrad(vf);
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
snGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvf,
  const word& name
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> SnGrad
  {
    fvc::snGrad(tvf(), name)
  };
  tvf.clear();
  return SnGrad;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
snGrad
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fvc::snGrad(vf, "snGrad(" + vf.name() + ')');
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>
snGrad
(
  const tmp<GeometricField<Type, fvPatchField, volMesh>>& tvf
)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> SnGrad
  {
    fvc::snGrad(tvf())
  };
  tvf.clear();
  return SnGrad;
}


}  // namespace fvc
}  // namespace mousse
