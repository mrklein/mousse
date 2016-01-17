// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_read_fields.hpp"
#include "hash_set.hpp"
#include "pstream.hpp"
#include "ioobject_list.hpp"

// Global Functions 
// Read all fields of type. Returns names of fields read. Guarantees all
// processors to read fields in same order.
template<class GeoField, class Mesh>
mousse::wordList mousse::ReadFields
(
  const Mesh& mesh,
  const IOobjectList& objects,
  PtrList<GeoField>& fields,
  const bool syncPar
)
{
  // Search list of objects for wanted type
  IOobjectList fieldObjects{objects.lookupClass(GeoField::typeName)};
  wordList masterNames{fieldObjects.names()};
  if (syncPar && Pstream::parRun())
  {
    // Check that I have the same fields as the master
    const wordList localNames(masterNames);
    Pstream::scatter(masterNames);
    HashSet<word> localNamesSet{localNames};
    FOR_ALL(masterNames, i)
    {
      const word& masterFld = masterNames[i];
      HashSet<word>::iterator iter = localNamesSet.find(masterFld);
      if (iter == localNamesSet.end())
      {
        FATAL_ERROR_IN
        (
          "ReadFields<class GeoField, class Mesh>"
          "(const Mesh&, const IOobjectList&, PtrList<GeoField>&"
          ", const bool)"
        )   << "Fields not synchronised across processors." << endl
          << "Master has fields " << masterNames
          << "  processor " << Pstream::myProcNo()
          << " has fields " << localNames << exit(FatalError);
      }
      else
      {
        localNamesSet.erase(iter);
      }
    }
    FOR_ALL_CONST_ITER(HashSet<word>, localNamesSet, iter)
    {
      FATAL_ERROR_IN
      (
        "ReadFields<class GeoField, class Mesh>"
        "(const Mesh&, const IOobjectList&, PtrList<GeoField>&"
        ", const bool)"
      )
      << "Fields not synchronised across processors." << endl
      << "Master has fields " << masterNames
      << "  processor " << Pstream::myProcNo()
      << " has fields " << localNames << exit(FatalError);
    }
  }
  fields.setSize(masterNames.size());
  // Make sure to read in masterNames order.
  FOR_ALL(masterNames, i)
  {
    Info<< "Reading " << GeoField::typeName << ' ' << masterNames[i]
      << endl;
    const IOobject& io = *fieldObjects[masterNames[i]];
    fields.set
    (
      i,
      new GeoField
      {
        IOobject
        {
          io.name(),
          io.instance(),
          io.local(),
          io.db(),
          IOobject::MUST_READ,
          IOobject::AUTO_WRITE,
          io.registerObject()
        },
        mesh
      }
    );
  }
  return masterNames;
}


template<class GeoField>
void mousse::ReadFields
(
  const word& fieldName,
  const typename GeoField::Mesh& mesh,
  const wordList& timeNames,
  objectRegistry& fieldsCache
)
{
  // Collect all times that are no longer used
  {
    HashSet<word> usedTimes{timeNames};
    DynamicList<word> unusedTimes{fieldsCache.size()};
    FOR_ALL_ITER(objectRegistry, fieldsCache, timeIter)
    {
      const word& tm = timeIter.key();
      if (!usedTimes.found(tm))
      {
        unusedTimes.append(tm);
      }
    }
    //Info<< "Unloading times " << unusedTimes << endl;
    FOR_ALL(unusedTimes, i)
    {
      objectRegistry& timeCache = const_cast<objectRegistry&>
      (
        fieldsCache.lookupObject<objectRegistry>(unusedTimes[i])
      );
      fieldsCache.checkOut(timeCache);
    }
  }
  // Load any new fields
  FOR_ALL(timeNames, i)
  {
    const word& tm = timeNames[i];
    // Create if not found
    if (!fieldsCache.found(tm))
    {
      //Info<< "Creating registry for time " << tm << endl;
      // Create objectRegistry if not found
      objectRegistry* timeCachePtr = new objectRegistry
      {
        IOobject
        {
          tm,
          tm,
          fieldsCache,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        }
      };
      timeCachePtr->store();
    }
    // Obtain cache for current time
    const objectRegistry& timeCache =
      fieldsCache.lookupObject<objectRegistry>
      (
        tm
      );
    // Store field if not found
    if (!timeCache.found(fieldName))
    {
      //Info<< "Loading field " << fieldName
      //    << " for time " << tm << endl;
      GeoField loadedFld
      {
        IOobject
        {
          fieldName,
          tm,
          mesh.thisDb(),
          IOobject::MUST_READ,
          IOobject::NO_WRITE,
          false
        },
        mesh
      };
      // Transfer to timeCache (new objectRegistry and store flag)
      GeoField* fldPtr = new GeoField
      {
        IOobject
        {
          fieldName,
          tm,
          timeCache,
          IOobject::NO_READ,
          IOobject::NO_WRITE
        },
        loadedFld
      };
      fldPtr->store();
    }
  }
}


template<class GeoField>
void mousse::ReadFields
(
  const word& fieldName,
  const typename GeoField::Mesh& mesh,
  const wordList& timeNames,
  const word& registryName
)
{
  ReadFields<GeoField>
  (
    fieldName,
    mesh,
    timeNames,
    const_cast<objectRegistry&>
    (
      mesh.thisDb().subRegistry(registryName, true)
    )
  );
}
