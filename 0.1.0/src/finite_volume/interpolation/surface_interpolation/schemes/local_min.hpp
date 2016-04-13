#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LOCAL_MIN_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LOCAL_MIN_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::localMin
// Description
//   LocalMin-mean differencing scheme class.
//   This scheme interpolates 1/field using a scheme specified at run-time
//   and return the reciprocal of the interpolate.

#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "time.hpp"


namespace mousse {

template<class Type>
class localMin
:
  public surfaceInterpolationScheme<Type>
{
public:
  //- Runtime type information
  TYPE_NAME("localMin");

  // Constructors
    //- Construct from mesh
    localMin(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    localMin
    (
      const fvMesh& mesh,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Construct from faceFlux and Istream
    localMin
    (
      const fvMesh& mesh,
      const surfaceScalarField& /*faceFlux*/,
      Istream&
    )
    :
      surfaceInterpolationScheme<Type>{mesh}
    {}

    //- Disallow default bitwise assignment
    localMin& operator=(const localMin&) = delete;
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      NOT_IMPLEMENTED
      (
        "localMin::weights"
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
          {
            "localMin::interpolate(" + vf.name() + ')',
            mesh.time().timeName(),
            mesh
          },
          mesh,
          vf.dimensions()
        }
      };
      GeometricField<Type, fvsPatchField, surfaceMesh>& vff = tvff();
      FOR_ALL(vff.boundaryField(), patchi) {
        vff.boundaryField()[patchi] = vf.boundaryField()[patchi];
      }
      const labelUList& own = mesh.owner();
      const labelUList& nei = mesh.neighbour();
      FOR_ALL(vff, facei) {
        vff[facei] = minMod(vf[own[facei]], vf[nei[facei]]);
      }
      return tvff;
    }
};

}  // namespace mousse

#endif

