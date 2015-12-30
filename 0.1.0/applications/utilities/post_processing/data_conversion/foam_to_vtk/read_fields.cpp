// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "read_fields.hpp"
#include "ioobject_list.hpp"
namespace mousse
{
// Global Functions 
template<class GeoField>
void readFields
(
  const vtkMesh& vMesh,
  const typename GeoField::Mesh& mesh,
  const IOobjectList& objects,
  const HashSet<word>& selectedFields,
  PtrList<GeoField>& fields
)
{
  // Search list of objects for volScalarFields
  IOobjectList fieldObjects(objects.lookupClass(GeoField::typeName));
  // Construct the vol scalar fields
  fields.setSize(fieldObjects.size());
  label nFields = 0;
  forAllIter(IOobjectList, fieldObjects, iter)
  {
    if (selectedFields.empty() || selectedFields.found(iter()->name()))
    {
      fields.set
      (
        nFields,
        vMesh.interpolate
        (
          GeoField
          (
            *iter(),
            mesh
          )
        )
      );
      nFields++;
    }
  }
  fields.setSize(nFields);
}
}  // namespace mousse
