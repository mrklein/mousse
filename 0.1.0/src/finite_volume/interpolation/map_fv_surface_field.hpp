// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#ifndef map_fv_surface_field_hpp_
#define map_fv_surface_field_hpp_
#include "field.hpp"
#include "surface_mesh.hpp"
namespace mousse
{
template<class Type, class MeshMapper>
class MapInternalField<Type, MeshMapper, surfaceMesh>
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
void MapInternalField<Type, MeshMapper, surfaceMesh>::operator()
(
  Field<Type>& field,
  const MeshMapper& mapper
) const
{
  if (field.size() != mapper.surfaceMap().sizeBeforeMapping())
  {
    FATAL_ERROR_IN
    (
      "void MapInternalField<Type, MeshMapper, surfaceMesh>::operator()\n"
      "(\n"
      "    Field<Type>& field,\n"
      "    const MeshMapper& mapper\n"
      ") const"
    )  << "Incompatible size before mapping.  Field size: " << field.size()
     << " map size: " << mapper.surfaceMap().sizeBeforeMapping()
     << abort(FatalError);
  }
  field.autoMap(mapper.surfaceMap());
  // Flip the flux
  const labelList flipFaces = mapper.surfaceMap().flipFaceFlux().toc();
  FOR_ALL(flipFaces, i)
  {
    if (flipFaces[i] < field.size())
    {
      field[flipFaces[i]] *= -1.0;
    }
  }
}
}  // namespace mousse
#endif
