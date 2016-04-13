#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_BLENDED_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_BLENDED_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::blended
// Description
//   linear/upwind blended differencing scheme.

#include "limited_surface_interpolation_scheme.hpp"
#include "blended_scheme_base.hpp"
#include "surface_fields.hpp"
#include "time.hpp"
#include "vol_fields.hpp"


namespace mousse {

template<class Type>
class blended
:
  public limitedSurfaceInterpolationScheme<Type>,
  public blendedSchemeBase<Type>
{
  // Private data
    const scalar blendingFactor_;
public:
  //- Runtime type information
  TYPE_NAME("blended");

  // Constructors
    //- Construct from mesh, faceFlux and blendingFactor
    blended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      const scalar blendingFactor
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, faceFlux},
      blendingFactor_{blendingFactor}
    {}

    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    blended
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, is},
      blendingFactor_{readScalar(is)}
    {}

    //- Construct from mesh, faceFlux and Istream
    blended
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>{mesh, faceFlux},
      blendingFactor_{readScalar(is)}
    {}

    //- Disallow default bitwise copy construct
    blended(const blended&) = delete;

    //- Disallow default bitwise assignment
    blended& operator=(const blended&) = delete;

  //- Destructor
  virtual ~blended()
  {}

  // Member Functions
    //- Return the face-based blending factor
    virtual tmp<surfaceScalarField> blendingFactor
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      return tmp<surfaceScalarField>
      {
        new surfaceScalarField
        {
          {
            vf.name() + "BlendingFactor",
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          },
          this->mesh(),
          dimensionedScalar
          {
            "blendingFactor",
            dimless,
            blendingFactor_
          }
        }
      };
    }

    //- Return the interpolation limiter
    virtual tmp<surfaceScalarField> limiter
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return tmp<surfaceScalarField>
      {
        new surfaceScalarField
        {
          {
            "blendedLimiter",
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          },
          this->mesh(),
          dimensionedScalar
          {
            "blendedLimiter",
            dimless,
            1 - blendingFactor_
          }
        }
      };
    }

    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return
        blendingFactor_*this->mesh().surfaceInterpolation::weights()
        + (1 - blendingFactor_)*pos(this->faceFlux_);
    }
};

}  // namespace mousse
#endif
