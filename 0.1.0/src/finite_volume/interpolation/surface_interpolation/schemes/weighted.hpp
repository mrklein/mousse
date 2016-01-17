// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::weighted
// Description
//   Interpolation scheme class using weights looked-up from the objectRegistry.
// SourceFiles
//   weighted.cpp
#ifndef weighted_hpp_
#define weighted_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class Type>
class weighted
:
  public surfaceInterpolationScheme<Type>
{
  // Private member data
    const surfaceScalarField& weights_;
public:
  //- Runtime type information
  TYPE_NAME("weighted");
  // Constructors
    //- Construct from weights
    weighted(const surfaceScalarField& weights)
    :
      surfaceInterpolationScheme<Type>{weights.mesh()},
      weights_{weights}
    {}
    //- Construct from Istream
    weighted(const fvMesh& mesh, Istream& is)
    :
      surfaceInterpolationScheme<Type>{mesh},
      weights_
      {
        this->mesh().objectRegistry::template
        lookupObject<const surfaceScalarField>
        (
          word(is)
        )
      }
    {}
    //- Construct from faceFlux and Istream
    weighted
    (
      const fvMesh& mesh,
      const surfaceScalarField&,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      weights_
      {
        this->mesh().objectRegistry::template
        lookupObject<const surfaceScalarField>
        (
          word{is}
        )
      }
    {}
    //- Disallow default bitwise assignment
    weighted&operator=(const weighted&) = delete;
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
}  // namespace mousse
#endif
