// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::harmonic
// Description
//   Harmonic-mean differencing scheme class.
//   This scheme interpolates 1/field using a scheme specified at run-time
//   and return the reciprocal of the interpolate.
// SourceFiles
//   harmonic.cpp
#ifndef harmonic_hpp_
#define harmonic_hpp_
#include "surface_interpolation_scheme.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "reverse_linear.hpp"
namespace mousse
{
class harmonic
:
  public surfaceInterpolationScheme<scalar>
{
  // Private Member Functions
    //- Disallow default bitwise assignment
    void operator=(const harmonic&);
public:
  //- Runtime type information
  TypeName("harmonic");
  // Constructors
    //- Construct from mesh
    harmonic(const fvMesh& mesh)
    :
      surfaceInterpolationScheme<scalar>(mesh)
    {}
    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    harmonic
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<scalar>(mesh)
    {}
    //- Construct from faceFlux and Istream
    harmonic
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& is
    )
    :
      surfaceInterpolationScheme<scalar>(mesh)
    {}
  // Member Functions
    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<scalar, fvPatchField, volMesh>&
    ) const
    {
      notImplemented
      (
        "harmonic::weights"
        "(const GeometricField<scalar, fvPatchField, volMesh>&)"
      );
      return tmp<surfaceScalarField>(NULL);
    }
    //- Return the face-interpolate of the given cell field
    virtual tmp<GeometricField<scalar, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const GeometricField<scalar, fvPatchField, volMesh>& vf
    ) const
    {
      return 1.0/(reverseLinear<scalar>(vf.mesh()).interpolate(1.0/vf));
    }
};
}  // namespace mousse
#endif
