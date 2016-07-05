#ifndef FINITE_VOLUME_FINITE_VOLUME_LAPLACIAN_SCHEMES_GAUSS_LAPLACIAN_SCHEME_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_LAPLACIAN_SCHEMES_GAUSS_LAPLACIAN_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussLaplacianScheme
// Description
//   Basic second-order laplacian using face-gradients and Gauss' theorem.

#include "laplacian_scheme.hpp"


namespace mousse {
namespace fv {

template<class Type, class GType>
class gaussLaplacianScheme
:
  public fv::laplacianScheme<Type, GType>
{
  // Private Member Functions
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh>> gammaSnGradCorr
    (
      const surfaceVectorField& SfGammaCorr,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
public:
  //- Runtime type information
  TYPE_NAME("Gauss");
  // Constructors
    //- Construct null
    gaussLaplacianScheme(const fvMesh& mesh)
    :
      laplacianScheme<Type, GType>{mesh}
    {}
    //- Construct from Istream
    gaussLaplacianScheme(const fvMesh& mesh, Istream& is)
    :
      laplacianScheme<Type, GType>{mesh, is}
    {}
    //- Construct from mesh, interpolation and snGradScheme schemes
    gaussLaplacianScheme
    (
      const fvMesh& mesh,
      const tmp<surfaceInterpolationScheme<GType>>& igs,
      const tmp<snGradScheme<Type>>& sngs
    )
    :
      laplacianScheme<Type, GType>{mesh, igs, sngs}
    {}
    //- Disallow default bitwise copy construct
    gaussLaplacianScheme(const gaussLaplacianScheme&) = delete;
    //- Disallow default bitwise assignment
    gaussLaplacianScheme& operator=(const gaussLaplacianScheme&) = delete;
  //- Destructor
  virtual ~gaussLaplacianScheme()
  {}
  // Member Functions
    static tmp<fvMatrix<Type>> fvmLaplacianUncorrected
    (
      const surfaceScalarField& gammaMagSf,
      const surfaceScalarField& deltaCoeffs,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcLaplacian
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type>> fvmLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh>> fvcLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
// Use macros to emulate partial-specialisation of the the Laplacian functions
// for scalar diffusivity gamma
#define DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(Type)                               \
                                                                              \
template<>                                                                    \
tmp<fvMatrix<Type>> gaussLaplacianScheme<Type, scalar>::fvmLaplacian          \
(                                                                             \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>&,                  \
  const GeometricField<Type, fvPatchField, volMesh>&                          \
);                                                                            \
                                                                              \
template<>                                                                    \
tmp<GeometricField<Type, fvPatchField, volMesh>>                              \
gaussLaplacianScheme<Type, scalar>::fvcLaplacian                              \
(                                                                             \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>&,                  \
  const GeometricField<Type, fvPatchField, volMesh>&                          \
);

DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(scalar);
DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(vector);
DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(sphericalTensor);
DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(symmTensor);
DEFINE_FVM_LAPLACIAN_SCALAR_GAMMA(tensor);

}  // namespace fv
}  // namespace mousse

#include "gauss_laplacian_scheme.ipp"

#endif
