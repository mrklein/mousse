#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_SURFACE_INTERPOLATION_SCHEME_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_LIMITED_SCHEMES_LIMITED_SURFACE_INTERPOLATION_SCHEME_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitedSurfaceInterpolationScheme
// Description
//   Abstract base class for limited surface interpolation schemes.

#include "surface_interpolation_scheme.hpp"
#include "fv_mesh.hpp"


namespace mousse {

template<class Type>
class limitedSurfaceInterpolationScheme
:
  public surfaceInterpolationScheme<Type>
{
protected:
  // Protected data
    const surfaceScalarField& faceFlux_;

public:

  //- Runtime type information
  TYPE_NAME("limitedSurfaceInterpolationScheme");

  // Declare run-time constructor selection tables

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      limitedSurfaceInterpolationScheme,
      Mesh,
      (
        const fvMesh& mesh,
        Istream& schemeData
      ),
      (mesh, schemeData)
    );

    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      tmp,
      limitedSurfaceInterpolationScheme,
      MeshFlux,
      (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
      ),
      (mesh, faceFlux, schemeData)
    );

  // Constructors

    //- Construct from mesh and faceFlux
    limitedSurfaceInterpolationScheme
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      faceFlux_{faceFlux}
    {}

    //- Construct from mesh and Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    limitedSurfaceInterpolationScheme
    (
      const fvMesh& mesh,
      Istream& is
    )
    :
      surfaceInterpolationScheme<Type>{mesh},
      faceFlux_
      {
        mesh.lookupObject<surfaceScalarField>
        (
          word{is}
        )
      }
    {}

    //- Disallow copy construct
    limitedSurfaceInterpolationScheme
    (
      const limitedSurfaceInterpolationScheme&
    ) = delete;

    //- Disallow default bitwise assignment
    limitedSurfaceInterpolationScheme& operator=
    (
      const limitedSurfaceInterpolationScheme&
    ) = delete;

  // Selectors

    //- Return new tmp interpolation scheme
    static tmp<limitedSurfaceInterpolationScheme<Type> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );

    //- Return new tmp interpolation scheme
    static tmp<limitedSurfaceInterpolationScheme<Type> > New
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );

  //- Destructor
  virtual ~limitedSurfaceInterpolationScheme();

  // Member Functions

    //- Return the interpolation weighting factors
    virtual tmp<surfaceScalarField> limiter
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;

    //- Return the interpolation weighting factors for the given field,
    //  by limiting the given weights with the given limiter
    tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const surfaceScalarField& CDweights,
      tmp<surfaceScalarField> tLimiter
    ) const;

    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

    //- Return the interpolation weighting factors
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    flux
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

};

}  // namespace mousse

// Add the patch constructor functions to the hash tables
#define MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, Type)               \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(SS<Type>, 0);                       \
                                                                              \
surfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >        \
  add##SS##Type##MeshConstructorToTable_;                                     \
                                                                              \
surfaceInterpolationScheme<Type>::addMeshFluxConstructorToTable<SS<Type> >    \
  add##SS##Type##MeshFluxConstructorToTable_;                                 \
                                                                              \
limitedSurfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> > \
  add##SS##Type##MeshConstructorToLimitedTable_;                              \
                                                                              \
limitedSurfaceInterpolationScheme<Type>::                                     \
  addMeshFluxConstructorToTable<SS<Type> >                                    \
  add##SS##Type##MeshFluxConstructorToLimitedTable_;


#define MAKELIMITED_SURFACE_INTERPOLATION_SCHEME(SS)                          \
                                                                              \
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, scalar)                     \
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, vector)                     \
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, sphericalTensor)            \
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, symmTensor)                 \
MAKELIMITED_SURFACE_INTERPOLATION_TYPE_SCHEME(SS, tensor)

#include "limited_surface_interpolation_scheme.ipp"

#endif
