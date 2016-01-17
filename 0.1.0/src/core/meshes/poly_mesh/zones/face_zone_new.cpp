// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "face_zone.hpp"
#include "dictionary.hpp"

// Member Functions 
mousse::autoPtr<mousse::faceZone> mousse::faceZone::New
(
  const word& name,
  const dictionary& dict,
  const label index,
  const faceZoneMesh& zm
)
{
  if (debug)
  {
    Info<< "faceZone::New(const word&, const dictionary&, const label, "
       "const faceZoneMesh&) : constructing faceZone " << name
      << endl;
  }
  const word zoneType(dict.lookup("type"));
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(zoneType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "faceZone::New(const word&, const dictionary&, "
      "const label, const faceZoneMesh&)",
      dict
    )
    << "Unknown faceZone type "
    << zoneType << nl << nl
    << "Valid faceZone types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }

  return autoPtr<faceZone>{cstrIter()(name, dict, index, zm)};
}
