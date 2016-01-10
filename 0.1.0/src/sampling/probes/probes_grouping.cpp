// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "probes.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "ioobject_list.hpp"
#include "string_list_ops.hpp"
// Private Member Functions 
void mousse::probes::clearFieldGroups()
{
  scalarFields_.clear();
  vectorFields_.clear();
  sphericalTensorFields_.clear();
  symmTensorFields_.clear();
  tensorFields_.clear();
  surfaceScalarFields_.clear();
  surfaceVectorFields_.clear();
  surfaceSphericalTensorFields_.clear();
  surfaceSymmTensorFields_.clear();
  surfaceTensorFields_.clear();
}
mousse::label mousse::probes::appendFieldGroup
(
  const word& fieldName,
  const word& fieldType
)
{
  if (fieldType == volScalarField::typeName)
  {
    scalarFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == volVectorField::typeName)
  {
    vectorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == volSphericalTensorField::typeName)
  {
    sphericalTensorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == volSymmTensorField::typeName)
  {
    symmTensorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == volTensorField::typeName)
  {
    tensorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == surfaceScalarField::typeName)
  {
    surfaceScalarFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == surfaceVectorField::typeName)
  {
    surfaceVectorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == surfaceSphericalTensorField::typeName)
  {
    surfaceSphericalTensorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == surfaceSymmTensorField::typeName)
  {
    surfaceSymmTensorFields_.append(fieldName);
    return 1;
  }
  else if (fieldType == surfaceTensorField::typeName)
  {
    surfaceTensorFields_.append(fieldName);
    return 1;
  }
  return 0;
}
mousse::label mousse::probes::classifyFields()
{
  label nFields = 0;
  clearFieldGroups();
  if (loadFromFiles_)
  {
    // check files for a particular time
    IOobjectList objects(mesh_, mesh_.time().timeName());
    wordList allFields = objects.sortedNames();
    labelList indices = findStrings(fieldSelection_, allFields);
    FOR_ALL(indices, fieldI)
    {
      const word& fieldName = allFields[indices[fieldI]];
      nFields += appendFieldGroup
      (
        fieldName,
        objects.find(fieldName)()->headerClassName()
      );
    }
  }
  else
  {
    // check currently available fields
    wordList allFields = mesh_.sortedNames();
    labelList indices = findStrings(fieldSelection_, allFields);
    FOR_ALL(indices, fieldI)
    {
      const word& fieldName = allFields[indices[fieldI]];
      nFields += appendFieldGroup
      (
        fieldName,
        mesh_.find(fieldName)()->type()
      );
    }
  }
  return nFields;
}
