// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvMeshTools
// Description
//   A collection of tools for operating on an fvMesh.
// SourceFiles
//   fv_mesh_tools.cpp
#ifndef fv_mesh_tools_hpp_
#define fv_mesh_tools_hpp_
#include "fv_mesh.hpp"
namespace mousse
{
class fvMeshTools
{
  template<class GeoField>
  static void addPatchFields
  (
    fvMesh&,
    const dictionary& patchFieldDict,
    const word& defaultPatchFieldType,
    const typename GeoField::value_type& defaultPatchValue
  );
  //- Set patchFields according to dictionary
  template<class GeoField>
  static void setPatchFields
  (
    fvMesh& mesh,
    const label patchI,
    const dictionary& patchFieldDict
  );
  //- Set patchFields to value
  template<class GeoField>
  static void setPatchFields
  (
    fvMesh& mesh,
    const label patchI,
    const typename GeoField::value_type& value
  );
  // Remove last patch fields
  template<class GeoField>
  static void trimPatchFields(fvMesh&, const label nPatches);
  template<class GeoField>
  static void reorderPatchFields(fvMesh&, const labelList& oldToNew);
  // Remove trialing patches
  static void trimPatches(fvMesh&, const label nPatches);
public:
  //- Add patch. Inserts patch before all processor patches.
  //  Supply per field the new patchField per field as a
  //  subdictionary or a default type. If validBoundary call is parallel
  //  synced and all add the same patch with same settings.
  static label addPatch
  (
    fvMesh& mesh,
    const polyPatch& patch,
    const dictionary& patchFieldDict,
    const word& defaultPatchFieldType,
    const bool validBoundary
  );
  //- Change patchField on registered fields according to dictionary
  static void setPatchFields
  (
    fvMesh& mesh,
    const label patchI,
    const dictionary& patchFieldDict
  );
  //- Change patchField to zero on registered fields
  static void zeroPatchFields(fvMesh& mesh, const label patchI);
  // -Reorder and remove trailing patches. If validBoundary call is parallel
  //  synced and all add the same patch with same settings
  static void reorderPatches
  (
    fvMesh&,
    const labelList& oldToNew,
    const label nPatches,
    const bool validBoundary
  );
};
}  // namespace mousse
#ifdef NoRepository
#   include "fv_mesh_tools_templates.cpp"
#endif
#endif
