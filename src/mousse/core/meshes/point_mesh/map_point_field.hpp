#ifndef CORE_MESHES_POINT_MESH_MAP_POINT_FIELD_HPP_
#define CORE_MESHES_POINT_MESH_MAP_POINT_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MapPointField
// Description
//   Map point field on topology change. This is a partial template
//   specialisation for GeoMesh=pointMesh
// See Also
//   MapGeometricFields

#include "field.hpp"
#include "point_mesh.hpp"


namespace mousse {

template<class Type, class MeshMapper>
class MapInternalField<Type, MeshMapper, pointMesh>
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


template<class Type, class MeshMapper>
void MapInternalField<Type, MeshMapper, pointMesh>::operator()
(
  Field<Type>& field,
  const MeshMapper& mapper
) const
{
  if (field.size() != mapper.pointMap().sizeBeforeMapping()) {
    FATAL_ERROR_IN
    (
      "void MapInternalField<Type, MeshMapper, pointMesh>::operator()\n"
      "(\n"
      "    Field<Type>& field,\n"
      "    const MeshMapper& mapper\n"
      ") const"
    )
    << "Incompatible size before mapping.  Field size: " << field.size()
    << " map size: " << mapper.pointMap().sizeBeforeMapping()
    << abort(FatalError);
  }
  field.autoMap(mapper.pointMap());
}

}  // namespace mousse
#endif

