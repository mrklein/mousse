// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linear
// Description
//   Central-differencing interpolation scheme class
// SourceFiles
//   linear.cpp

#ifndef linear_hpp_
#define linear_hpp_

#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"

namespace mousse
{
template<class Type>
class linear
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const linear&);
public:
  //- Runtime type information
  TYPE_NAME("linear");
  // Constructors
    //- Construct from mesh
    linear(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from Istream
    linear(const fvMesh& mesh, Istream&)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from faceFlux and Istream
    linear
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
      return this->mesh().surfaceInterpolation::weights();
    }
};
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
linearInterpolate(const GeometricField<Type, fvPatchField, volMesh>& vf)
{
  return surfaceInterpolationScheme<Type>::interpolate
  (
    vf,
    vf.mesh().surfaceInterpolation::weights()
  );
}
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
linearInterpolate(const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf)
{
  tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tinterp =
    linearInterpolate(tvf());
  tvf.clear();
  return tinterp;
}
}  // namespace mousse
#endif
