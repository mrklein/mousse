// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_laplacian_scheme.hpp"
#include "surface_interpolate.hpp"
#include "fvc_div.hpp"
#include "fvc_grad.hpp"
#include "fv_matrices.hpp"
namespace mousse
{
namespace fv
{
template<class Type, class GType>
tmp<fvMatrix<Type> >
gaussLaplacianScheme<Type, GType>::fvmLaplacianUncorrected
(
  const surfaceScalarField& gammaMagSf,
  const surfaceScalarField& deltaCoeffs,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  tmp<fvMatrix<Type> > tfvm
  (
    new fvMatrix<Type>
    (
      vf,
      deltaCoeffs.dimensions()*gammaMagSf.dimensions()*vf.dimensions()
    )
  );
  fvMatrix<Type>& fvm = tfvm();
  fvm.upper() = deltaCoeffs.internalField()*gammaMagSf.internalField();
  fvm.negSumDiag();
  forAll(vf.boundaryField(), patchi)
  {
    const fvPatchField<Type>& pvf = vf.boundaryField()[patchi];
    const fvsPatchScalarField& pGamma = gammaMagSf.boundaryField()[patchi];
    const fvsPatchScalarField& pDeltaCoeffs =
      deltaCoeffs.boundaryField()[patchi];
    if (pvf.coupled())
    {
      fvm.internalCoeffs()[patchi] =
        pGamma*pvf.gradientInternalCoeffs(pDeltaCoeffs);
      fvm.boundaryCoeffs()[patchi] =
       -pGamma*pvf.gradientBoundaryCoeffs(pDeltaCoeffs);
    }
    else
    {
      fvm.internalCoeffs()[patchi] = pGamma*pvf.gradientInternalCoeffs();
      fvm.boundaryCoeffs()[patchi] = -pGamma*pvf.gradientBoundaryCoeffs();
    }
  }
  return tfvm;
}
template<class Type, class GType>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
gaussLaplacianScheme<Type, GType>::gammaSnGradCorr
(
  const surfaceVectorField& SfGammaCorr,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = this->mesh();
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tgammaSnGradCorr
  (
    new GeometricField<Type, fvsPatchField, surfaceMesh>
    (
      IOobject
      (
        "gammaSnGradCorr("+vf.name()+')',
        vf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      SfGammaCorr.dimensions()
     *vf.dimensions()*mesh.deltaCoeffs().dimensions()
    )
  );
  for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
  {
    tgammaSnGradCorr().replace
    (
      cmpt,
      SfGammaCorr & fvc::interpolate(fvc::grad(vf.component(cmpt)))
    );
  }
  return tgammaSnGradCorr;
}
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussLaplacianScheme<Type, GType>::fvcLaplacian
(
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = this->mesh();
  tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
  (
    fvc::div(this->tsnGradScheme_().snGrad(vf)*mesh.magSf())
  );
  tLaplacian().rename("laplacian(" + vf.name() + ')');
  return tLaplacian;
}
template<class Type, class GType>
tmp<fvMatrix<Type> >
gaussLaplacianScheme<Type, GType>::fvmLaplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>& gamma,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = this->mesh();
  const surfaceVectorField Sn(mesh.Sf()/mesh.magSf());
  const surfaceVectorField SfGamma(mesh.Sf() & gamma);
  const GeometricField<scalar, fvsPatchField, surfaceMesh> SfGammaSn
  (
    SfGamma & Sn
  );
  const surfaceVectorField SfGammaCorr(SfGamma - SfGammaSn*Sn);
  tmp<fvMatrix<Type> > tfvm = fvmLaplacianUncorrected
  (
    SfGammaSn,
    this->tsnGradScheme_().deltaCoeffs(vf),
    vf
  );
  fvMatrix<Type>& fvm = tfvm();
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tfaceFluxCorrection
    = gammaSnGradCorr(SfGammaCorr, vf);
  if (this->tsnGradScheme_().corrected())
  {
    tfaceFluxCorrection() +=
      SfGammaSn*this->tsnGradScheme_().correction(vf);
  }
  fvm.source() -= mesh.V()*fvc::div(tfaceFluxCorrection())().internalField();
  if (mesh.fluxRequired(vf.name()))
  {
    fvm.faceFluxCorrectionPtr() = tfaceFluxCorrection.ptr();
  }
  return tfvm;
}
template<class Type, class GType>
tmp<GeometricField<Type, fvPatchField, volMesh> >
gaussLaplacianScheme<Type, GType>::fvcLaplacian
(
  const GeometricField<GType, fvsPatchField, surfaceMesh>& gamma,
  const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
  const fvMesh& mesh = this->mesh();
  const surfaceVectorField Sn(mesh.Sf()/mesh.magSf());
  const surfaceVectorField SfGamma(mesh.Sf() & gamma);
  const GeometricField<scalar, fvsPatchField, surfaceMesh> SfGammaSn
  (
    SfGamma & Sn
  );
  const surfaceVectorField SfGammaCorr(SfGamma - SfGammaSn*Sn);
  tmp<GeometricField<Type, fvPatchField, volMesh> > tLaplacian
  (
    fvc::div
    (
      SfGammaSn*this->tsnGradScheme_().snGrad(vf)
     + gammaSnGradCorr(SfGammaCorr, vf)
    )
  );
  tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');
  return tLaplacian;
}
}  // namespace fv
}  // namespace mousse
