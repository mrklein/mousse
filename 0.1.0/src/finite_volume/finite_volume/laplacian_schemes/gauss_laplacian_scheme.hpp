// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::gaussLaplacianScheme
// Description
//   Basic second-order laplacian using face-gradients and Gauss' theorem.
// SourceFiles
//   gauss_laplacian_scheme.cpp
#ifndef gauss_laplacian_scheme_hpp_
#define gauss_laplacian_scheme_hpp_
#include "laplacian_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type, class GType>
class gaussLaplacianScheme
:
  public fv::laplacianScheme<Type, GType>
{
  // Private Member Functions
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > gammaSnGradCorr
    (
      const surfaceVectorField& SfGammaCorr,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    //- Disallow default bitwise copy construct
    gaussLaplacianScheme(const gaussLaplacianScheme&);
    //- Disallow default bitwise assignment
    void operator=(const gaussLaplacianScheme&);
public:
  //- Runtime type information
  TypeName("Gauss");
  // Constructors
    //- Construct null
    gaussLaplacianScheme(const fvMesh& mesh)
    :
      laplacianScheme<Type, GType>(mesh)
    {}
    //- Construct from Istream
    gaussLaplacianScheme(const fvMesh& mesh, Istream& is)
    :
      laplacianScheme<Type, GType>(mesh, is)
    {}
    //- Construct from mesh, interpolation and snGradScheme schemes
    gaussLaplacianScheme
    (
      const fvMesh& mesh,
      const tmp<surfaceInterpolationScheme<GType> >& igs,
      const tmp<snGradScheme<Type> >& sngs
    )
    :
      laplacianScheme<Type, GType>(mesh, igs, sngs)
    {}
  //- Destructor
  virtual ~gaussLaplacianScheme()
  {}
  // Member Functions
    static tmp<fvMatrix<Type> > fvmLaplacianUncorrected
    (
      const surfaceScalarField& gammaMagSf,
      const surfaceScalarField& deltaCoeffs,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<fvMatrix<Type> > fvmLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcLaplacian
    (
      const GeometricField<GType, fvsPatchField, surfaceMesh>&,
      const GeometricField<Type, fvPatchField, volMesh>&
    );
};
// Use macros to emulate partial-specialisation of the the Laplacian functions
// for scalar diffusivity gamma
#define defineFvmLaplacianScalarGamma(Type)                                 \
                                      \
template<>                                                                  \
tmp<fvMatrix<Type> > gaussLaplacianScheme<Type, scalar>::fvmLaplacian       \
(                                                                           \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>&,              \
  const GeometricField<Type, fvPatchField, volMesh>&                      \
);                                                                          \
                                      \
template<>                                                                  \
tmp<GeometricField<Type, fvPatchField, volMesh> >                           \
gaussLaplacianScheme<Type, scalar>::fvcLaplacian                            \
(                                                                           \
  const GeometricField<scalar, fvsPatchField, surfaceMesh>&,              \
  const GeometricField<Type, fvPatchField, volMesh>&                      \
);
defineFvmLaplacianScalarGamma(scalar);
defineFvmLaplacianScalarGamma(vector);
defineFvmLaplacianScalarGamma(sphericalTensor);
defineFvmLaplacianScalarGamma(symmTensor);
defineFvmLaplacianScalarGamma(tensor);
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "gauss_laplacian_scheme.cpp"
#endif
#endif
