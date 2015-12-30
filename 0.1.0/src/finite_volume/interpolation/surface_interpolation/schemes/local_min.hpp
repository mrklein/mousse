// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::localMin
// Description
//   LocalMin-mean differencing scheme class.
//   This scheme interpolates 1/field using a scheme specified at run-time
//   and return the reciprocal of the interpolate.
// SourceFiles
//   local_min.cpp
#ifndef local_min_hpp_
#define local_min_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
namespace mousse
{
template<class Type>
class localMin
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const localMin&);
public:
  //- Runtime type information
  TypeName("localMin");
  // Constructors
    //- Construct from mesh
    localMin(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    localMin
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
    //- Construct from faceFlux and Istream
    localMin
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>(mesh)
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const
    {
      notImplemented
      (
        "localMin::weights"
        "(const GeometricField<Type, fvPatchField, volMesh>&)"
      );
      return tmp<surfaceScalarField>(NULL);
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
      (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
          IOobject
          (
            "localMin::interpolate(" + vf.name() + ')',
            mesh.time().timeName(),
            mesh
          ),
          mesh,
          vf.dimensions()
        )
      );
      GeometricField<Type, fvsPatchField, surfaceMesh>& vff = tvff();
      forAll(vff.boundaryField(), patchi)
      {
        vff.boundaryField()[patchi] = vf.boundaryField()[patchi];
      }
      const labelUList& own = mesh.owner();
      const labelUList& nei = mesh.neighbour();
      forAll(vff, facei)
      {
        vff[facei] = minMod(vf[own[facei]], vf[nei[facei]]);
      }
      return tvff;
    }
};
}  // namespace mousse
#endif
