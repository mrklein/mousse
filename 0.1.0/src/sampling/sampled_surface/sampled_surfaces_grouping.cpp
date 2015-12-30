// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "sampled_surfaces.hpp"
#include "vol_fields.hpp"
#include "ioobject_list.hpp"
#include "string_list_ops.hpp"
// Private Member Functions 
mousse::label mousse::sampledSurfaces::classifyFields()
{
  label nFields = 0;
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
        nFields += indices.size();
      }
      else
      {
        WarningIn("sampledSurfaces::classifyFields()")
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
        nFields += indices.size();
      }
      else
      {
        WarningIn("sampledSurfaces::classifyFields()")
          << "Cannot find registered field matching "
          << fieldSelection_[i] << endl;
      }
    }
  }
  return nFields;
}
