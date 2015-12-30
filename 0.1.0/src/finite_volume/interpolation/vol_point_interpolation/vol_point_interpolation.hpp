// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::volPointInterpolation
// Description
//   Interpolate from cell centres to points (vertices) using inverse distance
//   weighting
// SourceFiles
//   vol_point_interpolation.cpp
//   vol_point_interpolate.cpp
#ifndef vol_point_interpolation_hpp_
#define vol_point_interpolation_hpp_
#include "_mesh_object.hpp"
#include "scalar_list.hpp"
#include "vol_fields.hpp"
#include "point_fields.hpp"
namespace mousse
{
class fvMesh;
class pointMesh;
class volPointInterpolation
:
  public MeshObject<fvMesh, UpdateableMeshObject, volPointInterpolation>
{
  // Private data
    //- Interpolation scheme weighting factor array.
    scalarListList pointWeights_;
    // Boundary handling
      //- Boundary addressing
      autoPtr<primitivePatch> boundaryPtr_;
      //- Per boundary face whether is on non-coupled patch
      boolList boundaryIsPatchFace_;
      //- Per mesh(!) point whether is on non-coupled patch (on any
      //  processor)
      boolList isPatchPoint_;
      //- Per boundary point the weights per pointFaces.
      scalarListList boundaryPointWeights_;
  // Private Member Functions
    //- Construct addressing over all boundary faces
    void calcBoundaryAddressing();
    //- Make weights for internal and coupled-only boundarypoints
    void makeInternalWeights(scalarField& sumWeights);
    //- Make weights for points on uncoupled patches
    void makeBoundaryWeights(scalarField& sumWeights);
    //- Construct all point weighting factors
    void makeWeights();
    //- Helper: push master point data to collocated points
    template<class Type>
    void pushUntransformedData(List<Type>&) const;
    //- Get boundary field in same order as boundary faces. Field is
    //  zero on all coupled and empty patches
    template<class Type>
    tmp<Field<Type> > flatBoundaryField
    (
      const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const;
    //- Add separated contributions
    template<class Type>
    void addSeparated
    (
      GeometricField<Type, pointPatchField, pointMesh>&
    ) const;
    //- Disallow default bitwise copy construct
    volPointInterpolation(const volPointInterpolation&);
    //- Disallow default bitwise assignment
    void operator=(const volPointInterpolation&);
public:
  // Declare name of the class and its debug switch
  ClassName("volPointInterpolation");
  // Constructors
    //- Constructor given fvMesh and pointMesh.
    explicit volPointInterpolation(const fvMesh&);
  //- Destructor
  ~volPointInterpolation();
  // Member functions
    // Edit
      //- Update mesh topology using the morph engine
      void updateMesh(const mapPolyMesh&);
      //- Correct weighting factors for moving mesh.
      bool movePoints();
  // Interpolation functions
    //- Interpolate volField using inverse distance weighting
    //  returning pointField
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
    //- Interpolate tmp<volField> using inverse distance weighting
    //  returning pointField
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    ) const;
    //- Interpolate volField using inverse distance weighting
    //  returning pointField with the same patchField types. Assigns
    //  to any fixedValue boundary conditions to make them consistent
    //  with internal field
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
    (
      const GeometricField<Type, fvPatchField, volMesh>&,
      const wordList& patchFieldTypes
    ) const;
    //- Interpolate tmp<volField> using inverse distance weighting
    //  returning pointField with the same patchField types. Assigns
    //  to any fixedValue boundary conditions to make them consistent
    //  with internal field
    template<class Type>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
    (
      const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
      const wordList& patchFieldTypes
    ) const;
    // Low level
      //- Interpolate internal field from volField to pointField
      //  using inverse distance weighting
      template<class Type>
      void interpolateInternalField
      (
        const GeometricField<Type, fvPatchField, volMesh>&,
        GeometricField<Type, pointPatchField, pointMesh>&
      ) const;
      //- Interpolate boundary field without applying constraints/boundary
      //  conditions
      template<class Type>
      void interpolateBoundaryField
      (
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        GeometricField<Type, pointPatchField, pointMesh>& pf
      ) const;
      //- Interpolate boundary with constraints/boundary conditions
      template<class Type>
      void interpolateBoundaryField
      (
        const GeometricField<Type, fvPatchField, volMesh>& vf,
        GeometricField<Type, pointPatchField, pointMesh>& pf,
        const bool overrideFixedValue
      ) const;
      //- Interpolate from volField to pointField
      //  using inverse distance weighting
      template<class Type>
      void interpolate
      (
        const GeometricField<Type, fvPatchField, volMesh>&,
        GeometricField<Type, pointPatchField, pointMesh>&
      ) const;
      //- Interpolate volField using inverse distance weighting
      //  returning pointField with name. Optionally caches
      template<class Type>
      tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
      (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name,
        const bool cache
      ) const;
    // Interpolation for displacement (applies 2D correction)
      //- Interpolate from volField to pointField
      //  using inverse distance weighting
      void interpolateDisplacement
      (
        const volVectorField&,
        pointVectorField&
      ) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "vol_point_interpolate.cpp"
#endif
#endif
