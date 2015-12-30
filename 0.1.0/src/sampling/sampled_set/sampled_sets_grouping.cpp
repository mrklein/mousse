// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_sets.hpp"
#include "vol_fields.hpp"
#include "ioobject_list.hpp"
#include "string_list_ops.hpp"
// Private Member Functions 
void mousse::sampledSets::clearFieldGroups()
{
  scalarFields_.clear();
  vectorFields_.clear();
  sphericalTensorFields_.clear();
  symmTensorFields_.clear();
  tensorFields_.clear();
}
mousse::label mousse::sampledSets::appendFieldGroup
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
  return 0;
}
mousse::label mousse::sampledSets::classifyFields()
{
  label nFields = 0;
  clearFieldGroups();
  if (loadFromFiles_)
  {
    // Check files for a particular time
    IOobjectList objects(mesh_, mesh_.time().timeName());
    wordList allFields = objects.sortedNames();
    forAll(fieldSelection_, i)
    {
      labelList indices = findStrings(fieldSelection_[i], allFields);
      if (indices.size())
      {
        forAll(indices, fieldI)
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
        WarningIn("sampledSets::classifyFields()")
          << "Cannot find field file matching "
          << fieldSelection_[i] << endl;
      }
    }
  }
  else
  {
    // Check currently available fields
    wordList allFields = mesh_.sortedNames();
    labelList indices = findStrings(fieldSelection_, allFields);
    forAll(fieldSelection_, i)
    {
      labelList indices = findStrings(fieldSelection_[i], allFields);
      if (indices.size())
      {
        forAll(indices, fieldI)
        {
          const word& fieldName = allFields[indices[fieldI]];
          nFields += appendFieldGroup
          (
            fieldName,
            mesh_.find(fieldName)()->type()
          );
        }
      }
      else
      {
        WarningIn("sampledSets::classifyFields()")
          << "Cannot find registered field matching "
          << fieldSelection_[i] << endl;
      }
    }
  }
  return nFields;
}
