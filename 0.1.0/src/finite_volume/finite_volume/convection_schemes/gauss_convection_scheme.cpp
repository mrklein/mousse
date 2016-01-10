// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_convection_scheme.hpp"
#include "fvc_surface_integrate.hpp"
#include "fv_matrices.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
const surfaceInterpolationScheme<Type>&
gaussConvectionScheme<Type>::interpScheme() const
{
  return tinterpScheme_();
}
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
gaussConvectionScheme<Type>::interpolate
(
  const surfaceScalarField&,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return tinterpScheme_().interpolate(vf);
}
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
gaussConvectionScheme<Type>::flux
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  return faceFlux*interpolate(faceFlux, vf);
}
template<class Type>
tmp<fvMatrix<Type> >
gaussConvectionScheme<Type>::fvmDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  tmp<surfaceScalarField> tweights = tinterpScheme_().weights(vf);
  const surfaceScalarField& weights = tweights();
  tmp<fvMatrix<Type> > tfvm
  (
    new fvMatrix<Type>
    (
      vf,
      faceFlux.dimensions()*vf.dimensions()
    )
  );
  fvMatrix<Type>& fvm = tfvm();
  fvm.lower() = -weights.internalField()*faceFlux.internalField();
  fvm.upper() = fvm.lower() + faceFlux.internalField();
  fvm.negSumDiag();
  FOR_ALL(vf.boundaryField(), patchI)
  {
    const fvPatchField<Type>& psf = vf.boundaryField()[patchI];
    const fvsPatchScalarField& patchFlux = faceFlux.boundaryField()[patchI];
    const fvsPatchScalarField& pw = weights.boundaryField()[patchI];
    fvm.internalCoeffs()[patchI] = patchFlux*psf.valueInternalCoeffs(pw);
    fvm.boundaryCoeffs()[patchI] = -patchFlux*psf.valueBoundaryCoeffs(pw);
  }
  if (tinterpScheme_().corrected())
  {
    fvm += fvc::surfaceIntegrate(faceFlux*tinterpScheme_().correction(vf));
  }
  return tfvm;
}
template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussConvectionScheme<Type>::fvcDiv
(
  const surfaceScalarField& faceFlux,
  const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
  tmp<GeometricField<Type, fvPatchField, volMesh> > tConvection
  (
    fvc::surfaceIntegrate(flux(faceFlux, vf))
  );
  tConvection().rename
  (
    "convection(" + faceFlux.name() + ',' + vf.name() + ')'
  );
  return tConvection;
}
}  // namespace fv
}  // namespace mousse
