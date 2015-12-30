// mousse: CFD toolbox
// Copyright (C) 2012-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bounded_ddt_scheme.hpp"
#include "fvc_div.hpp"
#include "fvc_ddt.hpp"
#include "fv_matrices.hpp"
#include "fvm_sup.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedDdtScheme<Type>::fvcDdt
(
  const dimensioned<Type>& dt
)
{
  return scheme_().fvcDdt(dt);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedDdtScheme<Type>::fvcDdt
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvcDdt(vf);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedDdtScheme<Type>::fvcDdt
(
  const dimensionedScalar& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvcDdt(rho, vf);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedDdtScheme<Type>::fvcDdt
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvcDdt(rho, vf) - fvc::ddt(rho)*vf;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedDdtScheme<Type>::fvcDdt
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvcDdt(alpha, rho, vf) - fvc::ddt(alpha, rho)*vf;
}
template<class Type>
tmp<fvMatrix<Type> >
boundedDdtScheme<Type>::fvmDdt
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvmDdt(vf);
}
template<class Type>
tmp<fvMatrix<Type> >
boundedDdtScheme<Type>::fvmDdt
(
  const dimensionedScalar& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvmDdt(rho, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
boundedDdtScheme<Type>::fvmDdt
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvmDdt(rho, vf) - fvm::Sp(fvc::ddt(rho), vf);
}
template<class Type>
tmp<fvMatrix<Type> >
boundedDdtScheme<Type>::fvmDdt
(
  const volScalarField& alpha,
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().fvmDdt(alpha, rho, vf) - fvm::Sp(fvc::ddt(alpha, rho), vf);
}
template<class Type>
tmp<typename boundedDdtScheme<Type>::fluxFieldType>
boundedDdtScheme<Type>::fvcDdtUfCorr
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
  return scheme_().fvcDdtUfCorr(U, Uf);
}
template<class Type>
tmp<typename boundedDdtScheme<Type>::fluxFieldType>
boundedDdtScheme<Type>::fvcDdtPhiCorr
(
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const fluxFieldType& phi
)
{
  return scheme_().fvcDdtPhiCorr(U, phi);
}
template<class Type>
tmp<typename boundedDdtScheme<Type>::fluxFieldType>
boundedDdtScheme<Type>::fvcDdtUfCorr
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const GeometricField<Type, fvsPatchField, surfaceMesh>& Uf
)
{
  return scheme_().fvcDdtUfCorr(rho, U, Uf);
}
template<class Type>
tmp<typename boundedDdtScheme<Type>::fluxFieldType>
boundedDdtScheme<Type>::fvcDdtPhiCorr
(
  const volScalarField& rho,
  const GeometricField<Type, fvPatchField, volMesh>& U,
  const fluxFieldType& phi
)
{
  return scheme_().fvcDdtPhiCorr(rho, U, phi);
}
template<class Type>
tmp<surfaceScalarField> boundedDdtScheme<Type>::meshPhi
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  return scheme_().meshPhi(vf);
}
}  // namespace fv
}  // namespace mousse
