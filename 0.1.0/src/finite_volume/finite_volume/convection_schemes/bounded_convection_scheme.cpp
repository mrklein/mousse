// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "bounded_convection_scheme.hpp"
#include "fvc_surface_integrate.hpp"
#include "fv_matrices.hpp"
#include "fvm_sup.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
boundedConvectionScheme<Type>::interpolate
(
  const surfaceScalarField& phi,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return scheme_().interpolate(phi, vf);
}
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
boundedConvectionScheme<Type>::flux
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return scheme_().flux(faceFlux, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
boundedConvectionScheme<Type>::fvmDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return
    scheme_().fvmDiv(faceFlux, vf)
   - fvm::Sp(fvc::surfaceIntegrate(faceFlux), vf);
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
boundedConvectionScheme<Type>::fvcDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return
    scheme_().fvcDiv(faceFlux, vf)
   - fvc::surfaceIntegrate(faceFlux)*vf;
}
}  // namespace fv
}  // namespace mousse
