// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coordinate_system.hpp"
#include "dictionary.hpp"


// Member Functions 
mousse::autoPtr<mousse::coordinateSystem> mousse::coordinateSystem::New
(
  const objectRegistry& obr,
  const dictionary& dict
)
{
  const dictionary& coordDict = dict.subDict(typeName_());
  word coordType = coordDict.lookup("type");
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(coordType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "coordinateSystem::New(const objectRegistry&, const dictionary&)",
      dict
    )
    << "Unknown coordinateSystem type "
    << coordType << nl << nl
    << "Valid coordinateSystem types are :" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return autoPtr<coordinateSystem>(cstrIter()(obr, coordDict));
}


mousse::autoPtr<mousse::coordinateSystem> mousse::coordinateSystem::New
(
  const dictionary& dict
)
{
  const dictionary& coordDict = dict.subDict(typeName_());
  return autoPtr<coordinateSystem>{new coordinateSystem{coordDict}};
}


mousse::autoPtr<mousse::coordinateSystem> mousse::coordinateSystem::New
(
  Istream& is
)
{
  const word name(is);
  const dictionary dict(is);
  return autoPtr<coordinateSystem>{new coordinateSystem{name, dict}};
}

