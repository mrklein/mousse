// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "preserve_patch_types.hpp"
#include "poly_boundary_mesh_entries.hpp"
// Global Functions 
void mousse::preservePatchTypes
(
  const objectRegistry& obr,
  const word& meshInstance,
  const fileName& meshDir,
  const wordList& patchNames,
  PtrList<dictionary>& patchDicts,
  const word& defaultFacesName,
  word& defaultFacesType
)
{
  patchDicts.setSize(patchNames.size());
  dictionary patchDictionary;
  // Read boundary file as single dictionary
  {
    IOobject patchEntriesHeader
    (
      "boundary",
      meshInstance,
      meshDir,
      obr,
      IOobject::MUST_READ,
      IOobject::NO_WRITE,
      false
    );
    if (patchEntriesHeader.headerOk())
    {
      // Create a list of entries from the boundary file.
      polyBoundaryMeshEntries patchEntries(patchEntriesHeader);
      FOR_ALL(patchEntries, patchi)
      {
        patchDictionary.add(patchEntries[patchi]);
      }
    }
  }
  FOR_ALL(patchNames, patchi)
  {
    if (patchDictionary.found(patchNames[patchi]))
    {
      const dictionary& patchDict =
        patchDictionary.subDict(patchNames[patchi]);
      patchDicts.set(patchi, patchDict.clone());
      patchDicts[patchi].remove("nFaces");
      patchDicts[patchi].remove("startFace");
    }
  }
  if (patchDictionary.found(defaultFacesName))
  {
    const dictionary& patchDict =
      patchDictionary.subDict(defaultFacesName);
    patchDict.readIfPresent("geometricType", defaultFacesType);
  }
  Info<< nl << "Default patch type set to " << defaultFacesType << endl;
}
