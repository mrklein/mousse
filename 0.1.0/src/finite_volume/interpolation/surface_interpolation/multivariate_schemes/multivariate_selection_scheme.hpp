#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_SELECTION_SCHEME_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_SELECTION_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateSelectionScheme
// Description
//   Generic multi-variate discretisation scheme class for which any of the
//   NVD, CNVD or NVDV schemes may be selected for each variable.
// SourceFiles
//   multivariate_selection_scheme.cpp
#include "multivariate_surface_interpolation_scheme.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type>
class multivariateSelectionScheme
:
  public multivariateSurfaceInterpolationScheme<Type>
{
  // Private data
    dictionary schemes_;
    const surfaceScalarField& faceFlux_;
    surfaceScalarField weights_;
public:
  //- Runtime type information
  TYPE_NAME("multivariateSelection");
  // Constructors
    //- Construct for field, faceFlux and Istream
    multivariateSelectionScheme
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
    //- Disallow default bitwise copy construct
    multivariateSelectionScheme(const multivariateSelectionScheme&) = delete;
    //- Disallow default bitwise assignment
    multivariateSelectionScheme& operator=
    (
      const multivariateSelectionScheme&
    ) = delete;
  // Member Operators
    //- surfaceInterpolationScheme sub-class returned by operator(field)
    //  which is used as the interpolation scheme for the field
    class fieldScheme
    :
      public multivariateSurfaceInterpolationScheme<Type>::fieldScheme
    {
      // Private data
        const surfaceScalarField& weights_;
    public:
      // Constructors
        fieldScheme
        (
          const GeometricField<Type, fvPatchField, volMesh>& field,
          const surfaceScalarField& weights
        )
        :
          multivariateSurfaceInterpolationScheme<Type>::
            fieldScheme(field),
          weights_(weights)
        {}
      // Member Functions
        //- Return the interpolation weighting factors
        tmp<surfaceScalarField> weights
        (
          const GeometricField<Type, fvPatchField, volMesh>&
        ) const
        {
          return weights_;
        }
    };
    tmp<surfaceInterpolationScheme<Type> > operator()
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    ) const
    {
      return tmp<surfaceInterpolationScheme<Type> >
      (
        new fieldScheme(field, weights_)
      );
    }
};
}  // namespace mousse
#ifdef NoRepository
#   include "multivariate_selection_scheme.cpp"
#endif
#endif
