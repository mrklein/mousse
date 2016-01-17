// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fv::multivariateGaussConvectionScheme
// Description
//   Basic second-order convection using face-gradients and Gauss' theorem.
// SourceFiles
//   multivariate_gauss_convection_scheme.cpp
#ifndef multivariate_gauss_convection_scheme_hpp_
#define multivariate_gauss_convection_scheme_hpp_
#include "convection_scheme.hpp"
namespace mousse
{
namespace fv
{
template<class Type>
class multivariateGaussConvectionScheme
:
  public fv::convectionScheme<Type>
{
  // Private data
    tmp<multivariateSurfaceInterpolationScheme<Type> > tinterpScheme_;
public:
  //- Runtime type information
  TYPE_NAME("Gauss");
  // Constructors
    //- Construct from flux and Istream
    multivariateGaussConvectionScheme
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      convectionScheme<Type>(mesh, faceFlux),
      tinterpScheme_
      (
        multivariateSurfaceInterpolationScheme<Type>::New
        (
          mesh, fields, faceFlux, is
        )
      )
    {}
  // Member Functions
    tmp<multivariateSurfaceInterpolationScheme<Type> >
    interpolationScheme() const
    {
      return tinterpScheme_;
    }
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<fvMatrix<Type> > fvmDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDiv
    (
      const surfaceScalarField&,
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};
}  // namespace fv
}  // namespace mousse
#ifdef NoRepository
#   include "multivariate_gauss_convection_scheme.cpp"
#endif
#endif
