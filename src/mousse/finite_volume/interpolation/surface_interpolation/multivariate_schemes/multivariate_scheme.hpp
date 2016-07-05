#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_SCHEME_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_MULTIVARIATE_SCHEMES_MULTIVARIATE_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateScheme
// Description
//   Generic multi-variate discretisation scheme class which may be instantiated
//   for any of the NVD, CNVD or NVDV schemes.

#include "multivariate_surface_interpolation_scheme.hpp"
#include "surface_fields.hpp"


namespace mousse {

template<class Type, class Scheme>
class multivariateScheme
:
  public multivariateSurfaceInterpolationScheme<Type>,
  public Scheme::LimiterType
{
  // Private data
    const surfaceScalarField& faceFlux_;
    surfaceScalarField weights_;
public:
  //- Runtime type information
  TYPE_NAME("multivariateScheme");

  // Constructors
    //- Construct for field, faceFlux and Istream
    multivariateScheme
    (
      const fvMesh& mesh,
      const typename multivariateSurfaceInterpolationScheme<Type>::
        fieldTable& fields,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );

    //- Disallow default bitwise copy construct
    multivariateScheme(const multivariateScheme&) = delete;

    //- Disallow default bitwise assignment
    multivariateScheme& operator=(const multivariateScheme&) = delete;

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
          multivariateSurfaceInterpolationScheme<Type>::fieldScheme{field},
          weights_{weights}
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

    tmp<surfaceInterpolationScheme<Type>> operator()
    (
      const GeometricField<Type, fvPatchField, volMesh>& field
    ) const
    {
      return tmp<surfaceInterpolationScheme<Type>>
      {
        new fieldScheme{field, weights_}
      };
    }
};
}  // namespace mousse


// Add the patch constructor functions to the hash tables
#define MAKE_LIMITED_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME(SS, LIMITER)   \
typedef multivariateScheme                                                    \
<                                                                             \
  scalar,                                                                     \
  LimitedScheme<scalar, LIMITER<NVDTVD>, limitFuncs::magSqr>                  \
>                                                                             \
  multivariateScheme##LIMITER_;                                               \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  multivariateScheme##LIMITER_,                                               \
  #SS,                                                                        \
  0                                                                           \
);                                                                            \
                                                                              \
multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable  \
<                                                                             \
  multivariateScheme                                                          \
  <                                                                           \
    scalar,                                                                   \
    LimitedScheme<scalar, LIMITER<NVDTVD>, limitFuncs::magSqr>                \
  >                                                                           \
>                                                                             \
  addMultivariate##SS##ConstructorToTable_;


#define MAKE_L_LIMITED_MULTIVARIATE_SURFACE_INTERPOLATION_SCHEME\
(                                                                             \
  SS,                                                                         \
  LLIMITER,                                                                   \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  LIMFUNC                                                                     \
)                                                                             \
typedef multivariateScheme                                                    \
<                                                                             \
  scalar,                                                                     \
  LimitedScheme<scalar, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC>      \
>                                                                             \
  multivariateScheme##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_;                  \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
(                                                                             \
  multivariateScheme##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_,                  \
  #SS,                                                                        \
  0                                                                           \
);                                                                            \
                                                                              \
multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable  \
<                                                                             \
  multivariateScheme                                                          \
  <                                                                           \
    scalar,                                                                   \
    LimitedScheme<scalar, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC>    \
  >                                                                           \
>                                                                             \
  addMultivariate##SS##ConstructorToTable_;


#include "multivariate_scheme.ipp"

#endif
