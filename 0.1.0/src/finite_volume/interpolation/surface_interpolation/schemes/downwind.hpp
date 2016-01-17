// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::downwind
// Description
//   Downwind differencing scheme class.
// SourceFiles
//   downwind.cpp
#ifndef downwind_hpp_
#define downwind_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type>
class downwind
:
  public surfaceInterpolationScheme<Type>
{
  // Private data
    const surfaceScalarField& faceFlux_;
public:
  //- Runtime type information
  TYPE_NAME("downwind");
  // Constructors
    //- Construct from faceFlux
    downwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      faceFlux_{faceFlux}
    {}
    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    downwind
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      faceFlux_
      {
        mesh.lookupObject<surfaceScalarField>
        (
          word(is)
        )
      }
    {}
    //- Construct from faceFlux and Istream
    downwind
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      faceFlux_{faceFlux}
    {}
    //- Disallow default bitwise assignment
    downwind& operator=(const downwind&) = delete;
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      return neg(faceFlux_);
    }
};
}  // namespace mousse
#endif
