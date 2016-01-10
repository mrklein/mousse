// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_zone.hpp"
#include "dictionary.hpp"

// Member Functions 
mousse::autoPtr<mousse::pointZone> mousse::pointZone::New
(
  const word& name,
  const dictionary& dict,
  const label index,
  const pointZoneMesh& zm
)
{
  if (debug)
  {
    Info<< "pointZone::New(const word&, const dictionary&, const label, "
       "const pointZoneMesh&) : constructing pointZone " << name
      << endl;
  }
  const word zoneType(dict.lookup("type"));
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(zoneType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "pointZone::New(const word&, const dictionary&, "
      "const label, const pointZoneMesh&)",
      dict
    )
    << "Unknown pointZone type "
    << zoneType << nl << nl
    << "Valid pointZone types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }

  return autoPtr<pointZone>{cstrIter()(name, dict, index, zm)};
}
