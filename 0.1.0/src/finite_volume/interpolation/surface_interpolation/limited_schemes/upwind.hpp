// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::upwind
// Description
//   Upwind differencing scheme class.
// SourceFiles
//   upwind.cpp
#ifndef upwind_hpp_
#define upwind_hpp_
#include "limited_surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type>
class upwind
:
  public limitedSurfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const upwind&);
public:
  //- Runtime type information
  TypeName("upwind");
  // Constructors
    //- Construct from faceFlux
    upwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux)
    {}
    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    upwind
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, is)
    {}
    //- Construct from faceFlux and Istream
    upwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream&
    )
    :
      limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux)
    {}
  // Member Functions
    //- Return the interpolation limiter
    virtual tmp<surfaceScalarField> limiter
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return tmp<surfaceScalarField>
      (
        new surfaceScalarField
        (
          IOobject
          (
            "upwindLimiter",
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          ),
          this->mesh(),
          dimensionedScalar("upwindLimiter", dimless, 0.0)
        )
      );
    }
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights() const
    {
      return pos(this->faceFlux_);
    }
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return weights();
    }
};
}  // namespace mousse
#endif
