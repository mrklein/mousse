// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "gauss_laplacian_scheme.hpp"
#include "surface_fields.hpp"
#include "fv_mesh.hpp"
#include "fv_matrix.hpp"
#include "fvc.hpp"


MAKE_FV_LAPLACIAN_SCHEME(gaussLaplacianScheme)

#define DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(Type)                              \
                                                                              \
template<>                                                                    \
mousse::tmp<mousse::fvMatrix<mousse::Type>>                                   \
mousse::fv::gaussLaplacianScheme<mousse::Type, mousse::scalar>::fvmLaplacian  \
(                                                                             \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& gamma,            \
  const GeometricField<Type, fvPatchField, volMesh>& vf                       \
)                                                                             \
{                                                                             \
  const fvMesh& mesh = this->mesh();                                          \
                                                                              \
  GeometricField<scalar, fvsPatchField, surfaceMesh> gammaMagSf               \
  {                                                                           \
    gamma*mesh.magSf()                                                        \
  };                                                                          \
                                                                              \
  tmp<fvMatrix<Type>> tfvm = fvmLaplacianUncorrected                          \
  (                                                                           \
    gammaMagSf,                                                               \
    this->tsnGradScheme_().deltaCoeffs(vf),                                   \
    vf                                                                        \
  );                                                                          \
  fvMatrix<Type>& fvm = tfvm();                                               \
                                                                              \
  if (this->tsnGradScheme_().corrected()) {                                   \
    if (mesh.fluxRequired(vf.name())) {                                       \
      fvm.faceFluxCorrectionPtr() = new                                       \
      GeometricField<Type, fvsPatchField, surfaceMesh>                        \
      {                                                                       \
        gammaMagSf*this->tsnGradScheme_().correction(vf)                      \
      };                                                                      \
                                                                              \
      fvm.source() -=                                                         \
        mesh.V()*                                                             \
        fvc::div                                                              \
        (                                                                     \
          *fvm.faceFluxCorrectionPtr()                                        \
        )().internalField();                                                  \
    } else {                                                                  \
      fvm.source() -=                                                         \
        mesh.V()*                                                             \
        fvc::div                                                              \
        (                                                                     \
          gammaMagSf*this->tsnGradScheme_().correction(vf)                    \
        )().internalField();                                                  \
    }                                                                         \
  }                                                                           \
                                                                              \
  return tfvm;                                                                \
}                                                                             \
                                                                              \
                                                                              \
template<>                                                                    \
mousse::tmp<mousse::GeometricField<mousse::Type, mousse::fvPatchField, mousse::volMesh>>\
mousse::fv::gaussLaplacianScheme<mousse::Type, mousse::scalar>::fvcLaplacian  \
(                                                                             \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>& gamma,            \
  const GeometricField<Type, fvPatchField, volMesh>& vf                       \
)                                                                             \
{                                                                             \
  const fvMesh& mesh = this->mesh();                                          \
                                                                              \
  tmp<GeometricField<Type, fvPatchField, volMesh>> tLaplacian                 \
  {                                                                           \
    fvc::div(gamma*this->tsnGradScheme_().snGrad(vf)*mesh.magSf())            \
  };                                                                          \
                                                                              \
  tLaplacian().rename("laplacian(" + gamma.name() + ',' + vf.name() + ')');   \
                                                                              \
  return tLaplacian;                                                          \
}


DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(scalar);
DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(vector);
DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(sphericalTensor);
DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(symmTensor);
DECLARE_FVM_LAPLACIAN_SCALAR_GAMMA(tensor);

