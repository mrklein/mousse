// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMeshAdder
// Description
//   Adds two fvMeshes without using any polyMesh morphing.
//   Uses fvMeshAdder.
// SourceFiles
//   fv_mesh_adder.cpp
//   fv_mesh_adder_templates.cpp
#ifndef fv_mesh_adder_hpp_
#define fv_mesh_adder_hpp_
#include "poly_mesh_adder.hpp"
#include "vol_fields_fwd.hpp"
#include "fv_patch_fields_fwd.hpp"
#include "fvs_patch_fields_fwd.hpp"
#include "fv_patch_field_mapper.hpp"
namespace mousse
{
// Forward declaration of classes
class IOobject;
class faceCoupleInfo;
class IOobjectList;
class fvMesh;
class volMesh;
class surfaceMesh;
class mapAddedPolyMesh;
class fvMeshAdder
:
  public polyMeshAdder
{
private:
  // Private Member Functions
    //- Calculate map from new patch faces to old patch faces. -1 where
    //  could not map.
    static labelList calcPatchMap
    (
      const label oldStart,
      const label oldSize,
      const labelList& oldToNew,
      const polyPatch& newPatch,
      const label unmappedIndex
    );
    //- Update single volField.
    template<class Type>
    static void MapVolField
    (
      const mapAddedPolyMesh& meshMap,
      GeometricField<Type, fvPatchField, volMesh>& fld,
      const GeometricField<Type, fvPatchField, volMesh>& fldToAdd
    );
    //- Update single surfaceField.
    template<class Type>
    static void MapSurfaceField
    (
      const mapAddedPolyMesh& meshMap,
      GeometricField<Type, fvsPatchField, surfaceMesh>& fld,
      const GeometricField<Type, fvsPatchField, surfaceMesh>& fldToAdd
    );
public:
  // Member Functions
    //- Inplace add mesh to fvMesh. Maps all stored fields. Returns map.
    static autoPtr<mapAddedPolyMesh> add
    (
      fvMesh& mesh0,
      const fvMesh& mesh1,
      const faceCoupleInfo& coupleInfo,
      const bool validBoundary = true
    );
    //- Map all volFields of Type
    template<class Type>
    static void MapVolFields
    (
      const mapAddedPolyMesh&,
      const fvMesh& mesh,
      const fvMesh& meshToAdd
    );
    //- Map all surfaceFields of Type
    template<class Type>
    static void MapSurfaceFields
    (
      const mapAddedPolyMesh&,
      const fvMesh& mesh,
      const fvMesh& meshToAdd
    );
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_mesh_adder_templates.cpp"
#endif
#endif
