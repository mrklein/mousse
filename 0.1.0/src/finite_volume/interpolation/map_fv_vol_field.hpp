#ifndef FINITE_VOLUME_INTERPOLATION_MAP_FV_VOL_FIELD_HPP_
#define FINITE_VOLUME_INTERPOLATION_MAP_FV_VOL_FIELD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "field.hpp"
#include "vol_mesh.hpp"


namespace mousse {

template<class Type, class MeshMapper>
class MapInternalField<Type, MeshMapper, volMesh>
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
void MapInternalField<Type, MeshMapper, volMesh>::operator()
(
  Field<Type>& field,
  const MeshMapper& mapper
) const
{
  if (field.size() != mapper.volMap().sizeBeforeMapping()) {
    FATAL_ERROR_IN
    (
      "void MapInternalField<Type, MeshMapper, volMesh>::operator()\n"
      "(\n"
      "    Field<Type>& field,\n"
      "    const MeshMapper& mapper\n"
      ") const"
    )
    << "Incompatible size before mapping.  Field size: " << field.size()
    << " map size: " << mapper.volMap().sizeBeforeMapping()
    << abort(FatalError);
  }
  field.autoMap(mapper.volMap());
}

}  // namespace mousse
#endif
