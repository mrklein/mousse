#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_FIXED_BLENDED_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_FIXED_BLENDED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedBlended
// Description
//   Two-scheme fixed-blending differencing scheme.
//   Similar to localBlended but uses a single (global) constant blending
//   factor. The factor applies to the first scheme and 1-factor to the
//   second scheme.
// Note
//   Although a blending factor of 0 and 1 is permitted, it is more efficient
//   just to use the underlying scheme directly.
// SourceFiles
//   fixed_blended.cpp


#include "surface_interpolation_scheme.hpp"

namespace mousse
{
template<class Type>
class fixedBlended
:
  public surfaceInterpolationScheme<Type>
{
  // Private data
    const scalar blendingFactor_;
  // Private Member Functions
    //- Scheme 1
    tmp<surfaceInterpolationScheme<Type> > tScheme1_;
    //- Scheme 2
    tmp<surfaceInterpolationScheme<Type> > tScheme2_;
public:
  //- Runtime type information
  TYPE_NAME("fixedBlended");
  // Constructors
    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    fixedBlended
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      blendingFactor_{readScalar(is)},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      },
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, is)
      }
    {
      if (blendingFactor_ < 0 || blendingFactor_ > 1)
      {
        FATAL_IO_ERROR_IN("fixedBlended(const fvMesh&, Istream&)", is)
          << "coefficient = " << blendingFactor_
          << " should be >= 0 and <= 1"
          << exit(FatalIOError);
      }
      if (surfaceInterpolationScheme<Type>::debug)
      {
        Info
          << "fixedBlended: " << blendingFactor_
          << "*" << tScheme1_().type()
          << " + (1-" << blendingFactor_ << ")*"
          << tScheme2_().type()
          <<endl;
      }
    }
    //- Construct from mesh, faceFlux and Istream
    fixedBlended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      blendingFactor_{readScalar(is)},
      tScheme1_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      },
      tScheme2_
      {
        surfaceInterpolationScheme<Type>::New(mesh, faceFlux, is)
      }
    {
      if (blendingFactor_ < 0 || blendingFactor_ > 1)
      {
        FATAL_IO_ERROR_IN("fixedBlended(const fvMesh&, Istream&)", is)
          << "coefficient = " << blendingFactor_
          << " should be >= 0 and <= 1"
          << exit(FatalIOError);
      }
      if (surfaceInterpolationScheme<Type>::debug)
      {
        Info
          << "fixedBlended: " << blendingFactor_
          << "*" << tScheme1_().type()
          << " + (1-" << blendingFactor_ << ")*"
          << tScheme2_().type()
          <<endl;
      }
    }
    //- Disallow default bitwise copy construct
    fixedBlended(const fixedBlended&) = delete;
    //- Disallow default bitwise assignment
    fixedBlended& operator=(const fixedBlended&) = delete;
  // Member Functions
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField>
    weights
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return
        blendingFactor_*tScheme1_().weights(vf)
       + (scalar(1.0) - blendingFactor_)*tScheme2_().weights(vf);
    }
    //- Return the face-interpolate of the given cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return
        blendingFactor_*tScheme1_().interpolate(vf)
       + (scalar(1.0) - blendingFactor_)*tScheme2_().interpolate(vf);
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
      if (tScheme1_().corrected())
      {
        if (tScheme2_().corrected())
        {
          return
          (
            blendingFactor_
           * tScheme1_().correction(vf)
           + (scalar(1.0) - blendingFactor_)
           * tScheme2_().correction(vf)
          );
        }
        else
        {
          return
          (
            blendingFactor_
           * tScheme1_().correction(vf)
          );
        }
      }
      else if (tScheme2_().corrected())
      {
        return
        (
          (scalar(1.0) - blendingFactor_)
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
