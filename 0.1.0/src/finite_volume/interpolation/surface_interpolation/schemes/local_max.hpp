// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::localMax
// Description
//   LocalMax-mean differencing scheme class.
//   This scheme interpolates 1/field using a scheme specified at run-time
//   and return the reciprocal of the interpolate.
// SourceFiles
//   local_max.cpp
#ifndef local_max_hpp_
#define local_max_hpp_

#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"

namespace mousse
{
template<class Type>
class localMax
:
  public surfaceInterpolationScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("localMax");

  // Constructors
    //- Construct from mesh
    localMax(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    localMax
    (
      const fvMesh& mesh,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Construct from faceFlux and Istream
    localMax
    (
      const fvMesh& mesh,
      const surfaceScalarField& /*faceFlux*/,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Disallow default bitwise assignment
    localMax& operator=(const localMax&) = delete;

  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      NOT_IMPLEMENTED
      (
        "localMax::weights"
        "(const GeometricField<Type, fvPatchField, volMesh>&)"
      );
      return tmp<surfaceScalarField>{NULL};
    }

    //- Return the face-interpolate of the given cell field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const
    {
      const fvMesh& mesh = vf.mesh();
      tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tvff
      {
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        {
          IOobject
          {
            "localMax::interpolate(" + vf.name() + ')',
            mesh.time().timeName(),
            mesh
          },
          mesh,
          vf.dimensions()
        }
      };
      GeometricField<Type, fvsPatchField, surfaceMesh>& vff = tvff();
      FOR_ALL(vff.boundaryField(), patchi)
      {
        vff.boundaryField()[patchi] = vf.boundaryField()[patchi];
      }
      const labelUList& own = mesh.owner();
      const labelUList& nei = mesh.neighbour();
      FOR_ALL(vff, facei)
      {
        vff[facei] = max(vf[own[facei]], vf[nei[facei]]);
      }
      return tvff;
    }
};
}  // namespace mousse
#endif
