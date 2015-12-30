// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::surfaceInterpolationScheme
// Description
//   Abstract base class for surface interpolation schemes.
// SourceFiles
//   surface_interpolation_scheme.cpp
#ifndef surface_interpolation_scheme_hpp_
#define surface_interpolation_scheme_hpp_
#include "tmp.hpp"
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
#include "type_info.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
class fvMesh;
template<class Type>
class surfaceInterpolationScheme
:
  public refCount
{
  // Private data
    //- Hold reference to mesh
    const fvMesh& mesh_;
  // Private Member Functions
    //- Disallow copy construct
    surfaceInterpolationScheme(const surfaceInterpolationScheme&);
    //- Disallow default bitwise assignment
    void operator=(const surfaceInterpolationScheme&);
public:
  //- Runtime type information
  TypeName("surfaceInterpolationScheme");
  // Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
      tmp,
      surfaceInterpolationScheme,
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
      surfaceInterpolationScheme,
      MeshFlux,
      (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
      ),
      (mesh, faceFlux, schemeData)
    );
  // Constructors
    //- Construct from mesh
    surfaceInterpolationScheme(const fvMesh& mesh)
    :
      mesh_(mesh)
    {}
  // Selectors
    //- Return new tmp interpolation scheme
    static tmp<surfaceInterpolationScheme<Type> > New
    (
      const fvMesh& mesh,
      Istream& schemeData
    );
    //- Return new tmp interpolation scheme
    static tmp<surfaceInterpolationScheme<Type> > New
    (
      const fvMesh& mesh,
      const surfaceScalarField& faceFlux,
      Istream& schemeData
    );
  //- Destructor
  virtual ~surfaceInterpolationScheme();
  // Member Functions
    //- Return mesh reference
    const fvMesh& mesh() const
    {
      return mesh_;
    }
    //- Return the face-interpolate of the given cell field
    //  with the given owner and neighbour weigting factors
    static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const tmp<surfaceScalarField>&,
      const tmp<surfaceScalarField>&
    );
    //- Return the face-interpolate of the given cell field
    //  with the given weighting factors
    static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const tmp<surfaceScalarField>&
    );
    //- Return the interpolation weighting factors for the given field
    virtual tmp<surfaceScalarField> weights
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const = 0;
    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
      return false;
    }
    //- Return the explicit correction to the face-interpolate
    //  for the given field
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction(const GeometricField<Type, fvPatchField, volMesh>&) const
    {
      return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >(NULL);
    }
    //- Return the face-interpolate of the given cell field
    //  with explicit correction
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate(const GeometricField<Type, fvPatchField, volMesh>&) const;
    //- Return the face-interpolate of the given tmp cell field
    //  with explicit correction
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    interpolate
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    ) const;
};
}  // namespace mousse
// Add the patch constructor functions to the hash tables
#define makeSurfaceInterpolationTypeScheme(SS, Type)                           \
                                       \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                       \
surfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >         \
  add##SS##Type##MeshConstructorToTable_;                                    \
                                       \
surfaceInterpolationScheme<Type>::addMeshFluxConstructorToTable<SS<Type> >     \
  add##SS##Type##MeshFluxConstructorToTable_;
#define makeSurfaceInterpolationScheme(SS)                                     \
                                       \
makeSurfaceInterpolationTypeScheme(SS, scalar)                                 \
makeSurfaceInterpolationTypeScheme(SS, vector)                                 \
makeSurfaceInterpolationTypeScheme(SS, sphericalTensor)                        \
makeSurfaceInterpolationTypeScheme(SS, symmTensor)                             \
makeSurfaceInterpolationTypeScheme(SS, tensor)
#ifdef NoRepository
#   include "surface_interpolation_scheme.cpp"
#endif
#endif
