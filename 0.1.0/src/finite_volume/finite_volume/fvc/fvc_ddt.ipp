// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_ddt.hpp"
#include "fv_mesh.hpp"
#include "ddt_scheme.hpp"


namespace mousse {
namespace fvc {

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
ddt
(
  const dimensioned<Type> dt,
  const fvMesh& mesh
)
{
  return fv::ddtScheme<Type>::New
  (
    mesh,
    mesh.ddtScheme("ddt(" + dt.name() + ')')
  )().fvcDdt(dt);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
ddt
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return fv::ddtScheme<Type>::New
  (
    vf.mesh(),
    vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
  )().fvcDdt(vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
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
  )().fvcDdt(rho, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
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
  )().fvcDdt(rho, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
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
  )().fvcDdt(alpha, rho, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
ddt
(
  const one&,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return ddt(vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh>>
ddt
(
  const GeometricField<Type, fvPatchField, volMesh>& vf,
  const one&
)
{
  return ddt(vf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
  return fv::ddtScheme<Type>::New
  (
    U.mesh(),
    U.mesh().ddtScheme("ddt(" + U.name() + ')')
  )().fvcDdtUfCorr(U, Uf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField
  <
    typename flux<Type>::type,
    fvsPatchField,
    surfaceMesh
  >& phi
)
{
  return fv::ddtScheme<Type>::New
  (
    U.mesh(),
    U.mesh().ddtScheme("ddt(" + U.name() + ')')
  )().fvcDdtPhiCorr(U, phi);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
  return fv::ddtScheme<Type>::New
  (
    U.mesh(),
    U.mesh().ddtScheme("ddt(" + U.name() + ')')
  )().fvcDdtUfCorr(rho, U, Uf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh>>
ddtCorr
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField
  <
    typename flux<Type>::type,
    fvsPatchField,
    surfaceMesh
  >& phi
)
{
  return fv::ddtScheme<Type>::New
  (
    U.mesh(),
    U.mesh().ddtScheme("ddt(" + rho.name() + ',' + U.name() + ')')
  )().fvcDdtPhiCorr(rho, U, phi);
}


}  // namespace fvc
}  // namespace mousse
