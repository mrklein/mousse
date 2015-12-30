// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_d2dt2.hpp"
#include "fv_mesh.hpp"
#include "d2dt2_scheme.hpp"
namespace mousse
{
namespace fvc
{
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
d2dt2
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::d2dt2Scheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("d2dt2(" + vf.name() + ')')
  )().fvcD2dt2(vf);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
d2dt2
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::d2dt2Scheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("d2dt2(" + rho.name() + ',' + vf.name() + ')')
  )().fvcD2dt2(rho, vf);
}
}  // namespace fvc
}  // namespace mousse
