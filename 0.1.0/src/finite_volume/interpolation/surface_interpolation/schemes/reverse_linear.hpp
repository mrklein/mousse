// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reverseLinear
// Description
//   Inversed weight central-differencing interpolation scheme class.
//   Useful for inverse weighted and harmonic interpolations.
// SourceFiles
//   reverse_linear.cpp
#ifndef reverse_linear_hpp_
#define reverse_linear_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class Type>
class reverseLinear
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const reverseLinear&);
public:
  //- Runtime type information
  TypeName("reverseLinear");
  // Constructors
    //- Construct from mesh
    reverseLinear(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from Istream
    reverseLinear(const fvMesh& mesh, Istream&)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from faceFlux and Istream
    reverseLinear
    (
      const fvMesh& mesh,
      const surfaceScalarField&,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      const fvMesh& mesh = this->mesh();
      tmp<surfaceScalarField> tcdWeights
      (
        mesh.surfaceInterpolation::weights()
      );
      const surfaceScalarField& cdWeights = tcdWeights();
      tmp<surfaceScalarField> treverseLinearWeights
      (
        new surfaceScalarField
        (
          IOobject
          (
            "reverseLinearWeights",
            mesh.time().timeName(),
            mesh
          ),
          mesh,
          dimless
        )
      );
      surfaceScalarField& reverseLinearWeights = treverseLinearWeights();
      reverseLinearWeights.internalField() =
        1.0 - cdWeights.internalField();
      forAll(mesh.boundary(), patchI)
      {
        if (reverseLinearWeights.boundaryField()[patchI].coupled())
        {
          reverseLinearWeights.boundaryField()[patchI] =
            1.0 - cdWeights.boundaryField()[patchI];
        }
        else
        {
          reverseLinearWeights.boundaryField()[patchI] =
            cdWeights.boundaryField()[patchI];
        }
      }
      return treverseLinearWeights;
    }
};
}  // namespace mousse
#endif
