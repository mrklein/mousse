// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "distributed_tri_surface_mesh.hpp"
#include "tri_surface_fields.hpp"
#include "map_distribute.hpp"
// Member Functions 
//template<class Type>
//void mousse::distributedTriSurfaceMesh::getField
//(
//    const word& fieldName,
//    const List<pointIndexHit>& info,
//    List<Type>& values
//) const
//{
//    typedef DimensionedField<Type, triSurfaceGeoMesh> DimensionedSurfField;
//
//
//    // Get query data (= local index of triangle)
//    // ~~~~~~~~~~~~~~
//
//    labelList triangleIndex(info.size());
//    autoPtr<mapDistribute> mapPtr
//    (
//        calcLocalQueries
//        (
//            info,
//            triangleIndex
//        )
//    );
//    const mapDistribute& map = mapPtr();
//
//
//    // Do my tests
//    // ~~~~~~~~~~~
//
//    const DimensionedSurfField& fld = lookupObject<DimensionedSurfField>
//    (
//        fieldName
//    );
//    const triSurface& s = static_cast<const triSurface&>(*this);
//
//    values.setSize(triangleIndex.size());
//
//    forAll(triangleIndex, i)
//    {
//        label triI = triangleIndex[i];
//        values[i] = fld[triI];
//    }
//
//
//    // Send back results
//    // ~~~~~~~~~~~~~~~~~
//
//    map.reverseDistribute(info.size(), values);
//}
template<class Type>
void mousse::distributedTriSurfaceMesh::distributeFields
(
  const mapDistribute& map
)
{
  typedef DimensionedField<Type, triSurfaceGeoMesh> DimensionedSurfField;
  HashTable<DimensionedSurfField*> fields
  (
    objectRegistry::lookupClass<DimensionedSurfField>()
  );
  for
  (
    typename HashTable<DimensionedSurfField*>::iterator fieldIter =
      fields.begin();
    fieldIter != fields.end();
    ++fieldIter
  )
  {
    DimensionedSurfField& field = *fieldIter();
    label oldSize = field.size();
    map.distribute(field);
    if (debug)
    {
      Info<< "Mapped " << field.typeName << ' ' << field.name()
        << " from size " << oldSize << " to size " << field.size()
        << endl;
    }
  }
}
