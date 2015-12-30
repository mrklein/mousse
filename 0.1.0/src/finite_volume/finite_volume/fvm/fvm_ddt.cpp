// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "fv_matrix.hpp"
#include "ddt_scheme.hpp"
namespace mousse
{
namespace fvm
{
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::ddtScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
  )().fvmDdt(vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return ddt(vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const dimensionedScalar& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::ddtScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
  )().fvmDdt(rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::ddtScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
  )().fvmDdt(rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::ddtScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme
    (
      "ddt("
     + alpha.name() + ','
     + rho.name() + ','
     + vf.name() + ')'
    )
  )().fvmDdt(alpha, rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const one&,
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return ddt(vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const one&,
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return ddt(rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
ddt
(
  const volScalarField& alpha,
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return ddt(alpha, vf);
}
}  // namespace fvm
}  // namespace mousse
