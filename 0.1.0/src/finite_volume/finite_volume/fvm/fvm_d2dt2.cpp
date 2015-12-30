// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrix.hpp"
#include "d2dt2_scheme.hpp"
namespace mousse
{
namespace fvm
{
template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::d2dt2Scheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().d2dt2Scheme("d2dt2(" + vf.name() + ')')
  )().fvmD2dt2(vf);
}
template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
  const dimensionedScalar& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::d2dt2Scheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().d2dt2Scheme("d2dt2(" + rho.name() + ',' + vf.name() + ')')
  )().fvmD2dt2(rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::d2dt2Scheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().d2dt2Scheme("d2dt2(" + rho.name() + ',' + vf.name() + ')')
  )().fvmD2dt2(rho, vf);
}
}  // namespace fvm
}  // namespace mousse
