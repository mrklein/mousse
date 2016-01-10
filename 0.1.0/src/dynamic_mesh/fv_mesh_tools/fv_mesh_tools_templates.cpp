// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fv_mesh_tools.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
template<class GeoField>
void mousse::fvMeshTools::addPatchFields
(
  fvMesh& mesh,
  const dictionary& patchFieldDict,
  const word& defaultPatchFieldType,
  const typename GeoField::value_type& defaultPatchValue
)
{
  HashTable<GeoField*> flds
  (
    mesh.objectRegistry::lookupClass<GeoField>()
  );
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld =
      fld.boundaryField();
    label sz = bfld.size();
    bfld.setSize(sz+1);
    if (patchFieldDict.found(fld.name()))
    {
      bfld.set
      (
        sz,
        GeoField::PatchFieldType::New
        (
          mesh.boundary()[sz],
          fld.dimensionedInternalField(),
          patchFieldDict.subDict(fld.name())
        )
      );
    }
    else
    {
      bfld.set
      (
        sz,
        GeoField::PatchFieldType::New
        (
          defaultPatchFieldType,
          mesh.boundary()[sz],
          fld.dimensionedInternalField()
        )
      );
      bfld[sz] == defaultPatchValue;
    }
  }
}
template<class GeoField>
void mousse::fvMeshTools::setPatchFields
(
  fvMesh& mesh,
  const label patchI,
  const dictionary& patchFieldDict
)
{
  HashTable<GeoField*> flds
  (
    mesh.objectRegistry::lookupClass<GeoField>()
  );
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld =
      fld.boundaryField();
    if (patchFieldDict.found(fld.name()))
    {
      bfld.set
      (
        patchI,
        GeoField::PatchFieldType::New
        (
          mesh.boundary()[patchI],
          fld.dimensionedInternalField(),
          patchFieldDict.subDict(fld.name())
        )
      );
    }
  }
}
template<class GeoField>
void mousse::fvMeshTools::setPatchFields
(
  fvMesh& mesh,
  const label patchI,
  const typename GeoField::value_type& value
)
{
  HashTable<GeoField*> flds
  (
    mesh.objectRegistry::lookupClass<GeoField>()
  );
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld =
      fld.boundaryField();
    bfld[patchI] == value;
  }
}
// Remove last patch field
template<class GeoField>
void mousse::fvMeshTools::trimPatchFields(fvMesh& mesh, const label nPatches)
{
  HashTable<GeoField*> flds
  (
    mesh.objectRegistry::lookupClass<GeoField>()
  );
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    fld.boundaryField().setSize(nPatches);
  }
}
// Reorder patch field
template<class GeoField>
void mousse::fvMeshTools::reorderPatchFields
(
  fvMesh& mesh,
  const labelList& oldToNew
)
{
  HashTable<GeoField*> flds
  (
    mesh.objectRegistry::lookupClass<GeoField>()
  );
  FOR_ALL_ITER(typename HashTable<GeoField*>, flds, iter)
  {
    GeoField& fld = *iter();
    typename GeoField::GeometricBoundaryField& bfld =
      fld.boundaryField();
    bfld.reorder(oldToNew);
  }
}
