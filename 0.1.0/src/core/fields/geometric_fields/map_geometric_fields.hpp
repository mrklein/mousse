// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MapInternalField
// Description
//   Generic internal field mapper.  For "real" mapping, add template
//   specialisations for mapping of internal fields depending on mesh
//   type.

#ifndef map_geometric_fields_hpp_
#define map_geometric_fields_hpp_

#include "poly_mesh.hpp"
#include "time.hpp"

namespace mousse
{
template<class Type, class MeshMapper, class GeoMesh>
class MapInternalField
{
public:
  MapInternalField()
  {}
  void operator()
  (
    Field<Type>& field,
    const MeshMapper& mapper
  ) const;
};
//- Generic Geometric field mapper.
//  For "real" mapping, add template specialisations
//  for mapping of internal fields depending on mesh type.
template
<
  class Type,
  template<class> class PatchField,
  class MeshMapper,
  class GeoMesh
>
void MapGeometricFields
(
  const MeshMapper& mapper
)
{
  HashTable<const GeometricField<Type, PatchField, GeoMesh>*> fields
  (
    mapper.thisDb().objectRegistry::template
      lookupClass<GeometricField<Type, PatchField, GeoMesh> >()
  );
  // It is necessary to enforce that all old-time fields are stored
  // before the mapping is performed.  Otherwise, if the
  // old-time-level field is mapped before the field itself, sizes
  // will not match.
  for
  (
    typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
      iterator fieldIter = fields.begin();
    fieldIter != fields.end();
    ++fieldIter
  )
  {
    GeometricField<Type, PatchField, GeoMesh>& field =
      const_cast<GeometricField<Type, PatchField, GeoMesh>&>
      (*fieldIter());
    //Note: check can be removed once pointFields are actually stored on
    //      the pointMesh instead of now on the polyMesh!
    if (&field.mesh() == &mapper.mesh())
    {
      field.storeOldTimes();
    }
  }
  for
  (
    typename HashTable<const GeometricField<Type, PatchField, GeoMesh>*>::
      iterator fieldIter = fields.begin();
    fieldIter != fields.end();
    ++fieldIter
  )
  {
    GeometricField<Type, PatchField, GeoMesh>& field =
      const_cast<GeometricField<Type, PatchField, GeoMesh>&>
      (*fieldIter());
    if (&field.mesh() == &mapper.mesh())
    {
      if (polyMesh::debug)
      {
        Info<< "Mapping " << field.typeName << ' ' << field.name()
          << endl;
      }
      // Map the internal field
      MapInternalField<Type, MeshMapper, GeoMesh>()
      (
        field.internalField(),
        mapper
      );
      // Map the patch fields
      typename GeometricField<Type, PatchField, GeoMesh>
      ::GeometricBoundaryField& bfield = field.boundaryField();
      FOR_ALL(bfield, patchi)
      {
        // Cannot check sizes for patch fields because of
        // empty fields in FV and because point fields get their size
        // from the patch which has already been resized
        //
        bfield[patchi].autoMap(mapper.boundaryMap()[patchi]);
      }
      field.instance() = field.time().timeName();
    }
    else if (polyMesh::debug)
    {
      Info<< "Not mapping " << field.typeName << ' ' << field.name()
        << " since originating mesh differs from that of mapper."
        << endl;
    }
  }
}
}  // namespace mousse
#endif
