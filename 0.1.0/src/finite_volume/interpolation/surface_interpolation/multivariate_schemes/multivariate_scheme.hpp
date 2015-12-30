// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multivariateScheme
// Description
//   Generic multi-variate discretisation scheme class which may be instantiated
//   for any of the NVD, CNVD or NVDV schemes.
// SourceFiles
//   multivariate_scheme.cpp
#ifndef multivariate_scheme_hpp_
#define multivariate_scheme_hpp_
#include "multivariate_surface_interpolation_scheme.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type, class Scheme>
class multivariateScheme
:
  public multivariateSurfaceInterpolationScheme<Type>,
  public Scheme::LimiterType
{
  // Private data
    const surfaceScalarField& faceFlux_;
    surfaceScalarField weights_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    multivariateScheme(const multivariateScheme&);
    //- Disallow default bitwise assignment
    void operator=(const multivariateScheme&);
public:
  //- Runtime type information
  TypeName("multivariateScheme");
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
  // Member Operators
    //- surfaceInterpolationScheme sub-class returned by operator(field)
    //  which is used as the interpolation scheme for the field
    class fieldScheme
    :
      public multivariateSurfaceInterpolationScheme<Type>::
        fieldScheme
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
// Add the patch constructor functions to the hash tables
#define makeLimitedMultivariateSurfaceInterpolationScheme(SS, LIMITER)         \
typedef multivariateScheme                                                     \
<                                                                              \
  scalar,                                                                    \
  LimitedScheme<scalar, LIMITER<NVDTVD>, limitFuncs::magSqr>                 \
>                                                                              \
  multivariateScheme##LIMITER_;                                              \
defineTemplateTypeNameAndDebugWithName(multivariateScheme##LIMITER_, #SS, 0);  \
                                       \
multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable   \
<                                                                              \
  multivariateScheme                                                         \
  <                                                                          \
    scalar,                                                                \
    LimitedScheme<scalar, LIMITER<NVDTVD>, limitFuncs::magSqr>             \
  >                                                                          \
>                                                                              \
  addMultivariate##SS##ConstructorToTable_;
#define makeLLimitedMultivariateSurfaceInterpolationScheme\
(                                                                              \
  SS,                                                                        \
  LLIMITER,                                                                  \
  LIMITER,                                                                   \
  NVDTVD,                                                                    \
  LIMFUNC                                                                    \
)                                                                              \
typedef multivariateScheme                                                     \
<                                                                              \
  scalar,                                                                    \
  LimitedScheme<scalar, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC>     \
>                                                                              \
  multivariateScheme##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_;                 \
defineTemplateTypeNameAndDebugWithName                                         \
(                                                                              \
  multivariateScheme##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_,                 \
  #SS,                                                                       \
  0                                                                          \
);                                                                             \
                                       \
multivariateSurfaceInterpolationScheme<scalar>::addIstreamConstructorToTable   \
<                                                                              \
  multivariateScheme                                                         \
  <                                                                          \
    scalar,                                                                \
    LimitedScheme<scalar, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC> \
  >                                                                          \
>                                                                              \
  addMultivariate##SS##ConstructorToTable_;
#ifdef NoRepository
#   include "multivariate_scheme.cpp"
#endif
#endif
