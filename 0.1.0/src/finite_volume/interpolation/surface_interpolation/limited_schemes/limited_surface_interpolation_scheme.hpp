// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::limitedSurfaceInterpolationScheme
// Description
//   Abstract base class for limited surface interpolation schemes.
// SourceFiles
//   limited_surface_interpolation_scheme.cpp
#ifndef limited_surface_interpolation_scheme_hpp_
#define limited_surface_interpolation_scheme_hpp_
#include "surface_interpolation_scheme.hpp"
namespace mousse
{
template<class Type>
class limitedSurfaceInterpolationScheme
:
  public surfaceInterpolationScheme<Type>
{
  // Private Member Functions
    //- Disallow copy construct
    limitedSurfaceInterpolationScheme
    (
      const limitedSurfaceInterpolationScheme&
    );
    //- Disallow default bitwise assignment
    void operator=(const limitedSurfaceInterpolationScheme&);
protected:
  // Protected data
    const surfaceScalarField& faceFlux_;
public:
  //- Runtime type information
  TypeName("limitedSurfaceInterpolationScheme");
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
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
    declareRunTimeSelectionTable
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
      surfaceInterpolationScheme<Type>(mesh),
      faceFlux_(faceFlux)
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
      surfaceInterpolationScheme<Type>(mesh),
      faceFlux_
      (
        mesh.lookupObject<surfaceScalarField>
        (
          word(is)
        )
      )
    {}
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
#define makelimitedSurfaceInterpolationTypeScheme(SS, Type)                    \
                                       \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                       \
surfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >         \
  add##SS##Type##MeshConstructorToTable_;                                    \
                                       \
surfaceInterpolationScheme<Type>::addMeshFluxConstructorToTable<SS<Type> >     \
  add##SS##Type##MeshFluxConstructorToTable_;                                \
                                       \
limitedSurfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >  \
  add##SS##Type##MeshConstructorToLimitedTable_;                             \
                                       \
limitedSurfaceInterpolationScheme<Type>::                                      \
  addMeshFluxConstructorToTable<SS<Type> >                                   \
  add##SS##Type##MeshFluxConstructorToLimitedTable_;
#define makelimitedSurfaceInterpolationScheme(SS)                              \
                                       \
makelimitedSurfaceInterpolationTypeScheme(SS, scalar)                          \
makelimitedSurfaceInterpolationTypeScheme(SS, vector)                          \
makelimitedSurfaceInterpolationTypeScheme(SS, sphericalTensor)                 \
makelimitedSurfaceInterpolationTypeScheme(SS, symmTensor)                      \
makelimitedSurfaceInterpolationTypeScheme(SS, tensor)
#ifdef NoRepository
#   include "limited_surface_interpolation_scheme.cpp"
#endif
#endif
