// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_fields.hpp"
template<class Mesh, class GeoField>
void mousse::readFields
(
  const Mesh& mesh,
  const IOobjectList& objects,
  PtrList<GeoField>& fields
)
{
  // Search list of objects for fields of type GeomField
  IOobjectList fieldObjects{objects.lookupClass(GeoField::typeName)};
  // Remove the cellDist field
  IOobjectList::iterator celDistIter = fieldObjects.find("cellDist");
  if (celDistIter != fieldObjects.end())
  {
    fieldObjects.erase(celDistIter);
  }
  // Construct the fields
  fields.setSize(fieldObjects.size());
  label fieldI = 0;
  FOR_ALL_ITER(IOobjectList, fieldObjects, iter)
  {
    fields.set
    (
      fieldI++,
      new GeoField{*iter(), mesh}
    );
  }
}
