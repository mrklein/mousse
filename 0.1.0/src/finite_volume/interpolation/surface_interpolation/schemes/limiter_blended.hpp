// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limiterBlended
// Description
//   Blends two specified schemes using the limiter function provided by a
//   limitedSurfaceInterpolationScheme.
//   The limited scheme is specified first followed by the scheme to be scaled
//   by the limiter and then the scheme scaled by 1 - limiter e.g.
//   div(phi,U)      Gauss limiterBlended vanLeer linear linearUpwind grad(U);
// SourceFiles
//   limiter_blended.cpp
#ifndef limiter_blended_hpp_
#define limiter_blended_hpp_
#include "limited_surface_interpolation_scheme.hpp"
namespace mousse
{
template<class Type>
class limiterBlended
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Limited scheme providing the limiter
    tmp<limitedSurfaceInterpolationScheme<Type> > tLimitedScheme_;
    //- Scheme 1
    tmp<surfaceInterpolationScheme<Type> > tScheme1_;
    //- Scheme 2
    tmp<surfaceInterpolationScheme<Type> > tScheme2_;
    //- Disallow default bitwise copy construct
    limiterBlended(const limiterBlended&);
    //- Disallow default bitwise assignment
    void operator=(const limiterBlended&);
public:
  //- Runtime type information
  TypeName("limiterBlended");
  // Constructors
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    limiterBlended
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      tLimitedScheme_
      (
        limitedSurfaceInterpolationScheme<Type>::New(mesh, is)
      ),
      tScheme1_
      (
        surfaceInterpolationScheme<Type>::New(mesh, is)
      ),
      tScheme2_
      (
        surfaceInterpolationScheme<Type>::New(mesh, is)
      )
    {}
    //- Construct from mesh, faceFlux and Istream
    limiterBlended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      tLimitedScheme_
      (
        limitedSurfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      ),
      tScheme1_
      (
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      ),
      tScheme2_
      (
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      )
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      surfaceScalarField blendingFactor
      (
        tLimitedScheme_().limiter(vf)
      );
      return
        blendingFactor*tScheme1_().weights(vf)
       + (scalar(1) - blendingFactor)*tScheme2_().weights(vf);
    }
    //- Return the face-interpolate of the given cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate(const GeometricField<Type, fvPatchField, volMesh>& vf) const
    {
      surfaceScalarField blendingFactor
      (
        tLimitedScheme_().limiter(vf)
      );
      return
        blendingFactor*tScheme1_().interpolate(vf)
       + (scalar(1) - blendingFactor)*tScheme2_().interpolate(vf);
    }
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return tScheme1_().corrected() || tScheme2_().corrected();
    }
    //- Return the explicit correction to the face-interpolate
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      surfaceScalarField blendingFactor
      (
        tLimitedScheme_().limiter(vf)
      );
      if (tScheme1_().corrected())
      {
        if (tScheme2_().corrected())
        {
          return
          (
            blendingFactor
           * tScheme1_().correction(vf)
           + (scalar(1.0) - blendingFactor)
           * tScheme2_().correction(vf)
          );
        }
        else
        {
          return
          (
            blendingFactor
           * tScheme1_().correction(vf)
          );
        }
      }
      else if (tScheme2_().corrected())
      {
        return
        (
          (scalar(1.0) - blendingFactor)
         * tScheme2_().correction(vf)
        );
      }
      else
      {
        return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        (
          NULL
        );
      }
    }
};
}  // namespace mousse
#endif
