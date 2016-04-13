// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "cell_zone.hpp"
#include "dictionary.hpp"


// Member Functions 
mousse::autoPtr<mousse::cellZone> mousse::cellZone::New
(
  const word& name,
  const dictionary& dict,
  const label index,
  const cellZoneMesh& zm
)
{
  if (debug) {
    Info << "cellZone::New(const word&, const dictionary&, const label, "
            "const cellZoneMesh&) : constructing cellZone " << name
      << endl;
  }
  const word zoneType{dict.lookup("type")};
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(zoneType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "cellZone::New(const word&, const dictionary&, "
      "const label, const cellZoneMesh&)",
      dict
    )
    << "Unknown cellZone type "
    << zoneType << nl << nl
    << "Valid cellZone types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }

  return autoPtr<cellZone>{cstrIter()(name, dict, index, zm)};
}

