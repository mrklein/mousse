// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::midPoint
// Description
//   Mid-point interpolation (weighting factors = 0.5) scheme class.
// SourceFiles
//   mid_point.cpp
#ifndef mid_point_hpp_
#define mid_point_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class Type>
class midPoint
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const midPoint&);
public:
  //- Runtime type information
  TypeName("midPoint");
  // Constructors
    //- Construct from mesh
    midPoint(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from Istream
    midPoint(const fvMesh& mesh, Istream&)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from faceFlux and Istream
    midPoint
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
      tmp<surfaceScalarField> taw
      (
        new surfaceScalarField
        (
          IOobject
          (
            "midPointWeights",
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
          ),
          this->mesh(),
          dimensionedScalar("0.5", dimless, 0.5)
        )
      );
      surfaceScalarField::GeometricBoundaryField& awbf =
        taw().boundaryField();
      forAll(awbf, patchi)
      {
        if (!awbf[patchi].coupled())
        {
          awbf[patchi] = 1.0;
        }
      }
      return taw;
    }
};
}  // namespace mousse
#endif
