#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_SCHEME_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::LimitedScheme
// Description
//   Class to create NVD/TVD limited weighting-factors.
//   The particular differencing scheme class is supplied as a template
//   argument, the weight function of which is called by the weight function
//   of this class for the internal faces as well as faces of coupled
//   patches (e.g. processor-processor patches). The weight function is
//   supplied the central-differencing weighting factor, the face-flux, the
//   cell and face gradients (from which the normalised variable
//   distribution may be created) and the cell centre distance.
//   This code organisation is both neat and efficient, allowing for
//   convenient implementation of new schemes to run on parallelised cases.

#include "limited_surface_interpolation_scheme.hpp"
#include "limit_funcs.hpp"
#include "nvdtvd.hpp"
#include "nvdvtvdv.hpp"


namespace mousse {

template<class Type, class Limiter, template<class> class LimitFunc>
class LimitedScheme
:
  public limitedSurfaceInterpolationScheme<Type>,
  public Limiter
{
  // Private Member Functions
    //- Calculate the limiter
    void calcLimiter
    (
      const GeometricField<Type, fvPatchField, volMesh>& phi,
      surfaceScalarField& limiterField
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("LimitedScheme");

  typedef Limiter LimiterType;
  // Constructors
    //- Construct from mesh and faceFlux and limiter scheme
    LimitedScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      const Limiter& weight
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, faceFlux},
      Limiter{weight}
    {}

    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    LimitedScheme
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, is},
      Limiter{is}
    {}

    //- Construct from mesh, faceFlux and Istream
    LimitedScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, faceFlux},
      Limiter{is}
    {}

    //- Disallow default bitwise copy construct
    LimitedScheme(const LimitedScheme&) = delete;

    //- Disallow default bitwise assignment
    LimitedScheme& operator=(const LimitedScheme&) = delete;

  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> limiter
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};

}  // namespace mousse


// Add the patch constructor functions to the hash tables
#define MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME\
(                                                                             \
  SS,                                                                         \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  LIMFUNC,                                                                    \
  TYPE                                                                        \
)                                                                             \
                                                                              \
typedef LimitedScheme<TYPE, LIMITER<NVDTVD>, limitFuncs::LIMFUNC>             \
  LimitedScheme##TYPE##LIMITER##NVDTVD##LIMFUNC##_;                           \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
  (LimitedScheme##TYPE##LIMITER##NVDTVD##LIMFUNC##_, #SS, 0);                 \
                                                                              \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                   \
<LimitedScheme<TYPE, LIMITER<NVDTVD>, limitFuncs::LIMFUNC> >                  \
  add##SS##LIMFUNC##TYPE##MeshConstructorToTable_;                            \
                                                                              \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable               \
<LimitedScheme<TYPE, LIMITER<NVDTVD>, limitFuncs::LIMFUNC> >                  \
  add##SS##LIMFUNC##TYPE##MeshFluxConstructorToTable_;                        \
                                                                              \
limitedSurfaceInterpolationScheme<TYPE>::addMeshConstructorToTable            \
<LimitedScheme<TYPE, LIMITER<NVDTVD>, limitFuncs::LIMFUNC> >                  \
  add##SS##LIMFUNC##TYPE##MeshConstructorToLimitedTable_;                     \
                                                                              \
limitedSurfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable        \
<LimitedScheme<TYPE, LIMITER<NVDTVD>, limitFuncs::LIMFUNC> >                  \
  add##SS##LIMFUNC##TYPE##MeshFluxConstructorToLimitedTable_;

#define MAKE_LIMITED_SURFACE_INTERPOLATION_SCHEME(SS, LIMITER)                \
                                                                              \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME                                \
(                                                                             \
  SS,                                                                         \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  magSqr,                                                                     \
  scalar                                                                      \
)                                                                             \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME                                \
(                                                                             \
  SS,                                                                         \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  magSqr,                                                                     \
  vector                                                                      \
)                                                                             \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME                                \
(                                                                             \
  SS,                                                                         \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  magSqr,                                                                     \
  sphericalTensor                                                             \
)                                                                             \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME                                \
(                                                                             \
  SS,                                                                         \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  magSqr,                                                                     \
  symmTensor                                                                  \
)                                                                             \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS,LIMITER,NVDTVD,magSqr,tensor)

#define MAKE_LIMITED_V_SURFACE_INTERPOLATION_SCHEME(SS, LIMITER)              \
MAKE_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS,LIMITER,NVDVTVDV,null,vector)

#define MAKE_L_LIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME\
(                                                                             \
  SS,                                                                         \
  LLIMITER,                                                                   \
  LIMITER,                                                                    \
  NVDTVD,                                                                     \
  LIMFUNC,                                                                    \
  TYPE                                                                        \
)                                                                             \
                                                                              \
typedef LimitedScheme<TYPE, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC>  \
  LimitedScheme##TYPE##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_;                 \
DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG_WITH_NAME                                 \
  (LimitedScheme##TYPE##LLIMITER##LIMITER##NVDTVD##LIMFUNC##_, #SS, 0);       \
                                                                              \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                   \
<LimitedScheme<TYPE, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC> >       \
  add##SS##LIMFUNC##TYPE##MeshConstructorToTable_;                            \
                                                                              \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable               \
<LimitedScheme<TYPE, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC> >       \
  add##SS##LIMFUNC##TYPE##MeshFluxConstructorToTable_;                        \
                                                                              \
limitedSurfaceInterpolationScheme<TYPE>::addMeshConstructorToTable            \
<LimitedScheme<TYPE, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC> >       \
  add##SS##LIMFUNC##TYPE##MeshConstructorToLimitedTable_;                     \
                                                                              \
limitedSurfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable        \
<LimitedScheme<TYPE, LLIMITER<LIMITER<NVDTVD> >, limitFuncs::LIMFUNC> >       \
  add##SS##LIMFUNC##TYPE##MeshFluxConstructorToLimitedTable_;


#include "limited_scheme.ipp"

#endif
