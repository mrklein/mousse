// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::clippedLinear
// Description
//   Central-differencing interpolation scheme using clipped-weights to
//   improve stability on meshes with very rapid variations in cell size.
// SourceFiles
//   clipped_linear.cpp
#ifndef clipped_linear_hpp_
#define clipped_linear_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
namespace mousse
{
template<class Type>
class clippedLinear
:
  public surfaceInterpolationScheme<Type>
{
  // Private data
    const scalar cellSizeRatio_;
    scalar wfLimit_;
  // Private Member Functions
    void calcWfLimit()
    {
      if (cellSizeRatio_ <= 0 || cellSizeRatio_ > 1)
      {
        FatalErrorIn("clippedLinear::calcWfLimit()")
          << "Given cellSizeRatio of " << cellSizeRatio_
          << " is not between 0 and 1"
          << exit(FatalError);
      }
      wfLimit_ = cellSizeRatio_/(1.0 + cellSizeRatio_);
    }
    //- Disallow default bitwise assignment
    void operator=(const clippedLinear&);
public:
  //- Runtime type information
  TypeName("clippedLinear");
  // Constructors
    //- Construct from mesh and cellSizeRatio
    clippedLinear(const fvMesh& mesh, const scalar cellSizeRatio)
    :
      surfaceInterpolationScheme<Type>(mesh),
      cellSizeRatio_(cellSizeRatio)
    {
      calcWfLimit();
    }
    //- Construct from Istream
    clippedLinear(const fvMesh& mesh, Istream& is)
    :
      surfaceInterpolationScheme<Type>(mesh),
      cellSizeRatio_(readScalar(is))
    {
      calcWfLimit();
    }
    //- Construct from faceFlux and Istream
    clippedLinear
    (
      const fvMesh& mesh,
      const surfaceScalarField&,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh),
      cellSizeRatio_(readScalar(is))
    {
      calcWfLimit();
    }
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
      tmp<surfaceScalarField> tclippedLinearWeights
      (
        new surfaceScalarField
        (
          IOobject
          (
            "clippedLinearWeights",
            mesh.time().timeName(),
            mesh
          ),
          mesh,
          dimless
        )
      );
      surfaceScalarField& clippedLinearWeights = tclippedLinearWeights();
      clippedLinearWeights.internalField() =
        max(min(cdWeights.internalField(), 1 - wfLimit_), wfLimit_);
      forAll(mesh.boundary(), patchi)
      {
        if (clippedLinearWeights.boundaryField()[patchi].coupled())
        {
          clippedLinearWeights.boundaryField()[patchi] =
            max
            (
              min
              (
                cdWeights.boundaryField()[patchi],
                1 - wfLimit_
              ),
              wfLimit_
            );
        }
        else
        {
          clippedLinearWeights.boundaryField()[patchi] =
            cdWeights.boundaryField()[patchi];
        }
      }
      return tclippedLinearWeights;
    }
};
}  // namespace mousse
#endif
