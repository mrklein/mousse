// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::pointLinear
// Description
//   Face-point interpolation scheme class derived from linear and
//   returns linear weighting factors but also applies an explicit correction.
//   Uses volPointInterpolation to obtain the field values at the face-points.
// SourceFiles
//   point_linear.cpp
#ifndef point_linear_hpp_
#define point_linear_hpp_
#include "linear.hpp"
namespace mousse
{
template<class Type>
class pointLinear
:
  public linear<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("pointLinear");
  // Constructors
    //- Construct from mesh
    pointLinear(const fvMesh& mesh)
    :
      linear<Type>{mesh}
    {}
    //- Construct from mesh and Istream
    pointLinear
    (
      const fvMesh& mesh,
      Istream&
    )
    :
      linear<Type>{mesh}
    {}
    //- Construct from mesh, faceFlux and Istream
    pointLinear
    (
      const fvMesh& mesh,
      const surfaceScalarField&,
      Istream&
    )
    :
      linear<Type>{mesh}
    {}
    //- Disallow default bitwise copy construct
    pointLinear(const pointLinear&) = delete;
    //- Disallow default bitwise assignment
    pointLinear& operator=(const pointLinear&) = delete;
  // Member Functions
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return true;
    }
    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const;
};
}  // namespace mousse
#endif
