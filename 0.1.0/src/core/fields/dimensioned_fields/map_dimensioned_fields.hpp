#ifndef CORE_FIELDS_DIMENSIONED_FIELDS_MAP_DIMENSIONED_FIELDS_HPP_
#define CORE_FIELDS_DIMENSIONED_FIELDS_MAP_DIMENSIONED_FIELDS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_mesh.hpp"
#include "map_fv_vol_field.hpp"
namespace mousse
{
template<class Type, class MeshMapper, class GeoMesh>
void MapDimensionedFields(const MeshMapper& mapper)
{
  typedef DimensionedField<Type, GeoMesh> FieldType;
  typedef HashTable<const FieldType*> TableType;
  TableType fields(mapper.thisDb().template lookupClass<FieldType>(true));
  FOR_ALL_CONST_ITER(typename TableType, fields, fieldIter)
  {
    FieldType& field = const_cast<FieldType&>(*fieldIter());
    if (&field.mesh() == &mapper.mesh())
    {
      if (polyMesh::debug)
      {
        Info<< "Mapping " << field.typeName << ' ' << field.name()
          << endl;
      }
      MapInternalField<Type, MeshMapper, GeoMesh>()(field, mapper);
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
