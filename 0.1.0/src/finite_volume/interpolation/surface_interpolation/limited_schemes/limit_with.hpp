#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMIT_WITH_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMIT_WITH_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitWith
// Description
//   limitWith differencing scheme limits the specified scheme with the
//   specified limiter.
// SourceFiles
//   limit_with.cpp


#include "surface_interpolation_scheme.hpp"
#include "limited_surface_interpolation_scheme.hpp"

namespace mousse
{
template<class Type>
class limitWith
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Interpolation scheme
    tmp<surfaceInterpolationScheme<Type> > tInterp_;
    //- Limiter
    tmp<limitedSurfaceInterpolationScheme<Type> > tLimiter_;
public:
  //- Runtime type information
  TYPE_NAME("limitWith");

  // Constructors
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    limitWith
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      tInterp_{surfaceInterpolationScheme<Type>::New(mesh, is)},
      tLimiter_{limitedSurfaceInterpolationScheme<Type>::New(mesh, is)}
    {}

    //- Construct from mesh, faceFlux and Istream
    limitWith
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      tInterp_{surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)},
      tLimiter_
      {
        limitedSurfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      }
    {}

    //- Disallow default bitwise copy construct
    limitWith(const limitWith&) = delete;

    //- Disallow default bitwise assignment
    limitWith& operator=(const limitWith&) = delete;

  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return tLimiter_().weights
      (
        vf,
        tInterp_().weights(vf),
        tLimiter_().limiter(vf)
      );
    }

    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return tInterp_().corrected();
    }

    //- Return the explicit correction to the face-interpolate
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction(const GeometricField<Type, fvPatchField, volMesh>& vf) const
    {
      return tLimiter_().limiter(vf)*tInterp_().correction(vf);
    }
};
}  // namespace mousse
#endif
