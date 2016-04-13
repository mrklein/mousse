// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "coordinate_rotation.hpp"
#include "object_registry.hpp"


// Selectors
mousse::autoPtr<mousse::coordinateRotation> mousse::coordinateRotation::New
(
  const dictionary& dict, const objectRegistry& obr
)
{
  if (debug) {
    Pout << "coordinateRotation::New(const dictionary&) : "
      << "constructing coordinateRotation"
      << endl;
  }
  word rotType = dict.lookup("type");
  objectRegistryConstructorTable::iterator cstrIter =
    objectRegistryConstructorTablePtr_->find(rotType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "coordinateRotation::New"
      "("
      "   const dictionary&, "
      "   const objectRegistry& "
      ")",
      dict
    )
    << "Unknown coordinateRotation type "
    << rotType << nl << nl
    << "Valid coordinateRotation types are :" <<  nl
    << "[default: axes ]"
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return autoPtr<coordinateRotation>{cstrIter()(dict, obr)};
}


mousse::autoPtr<mousse::coordinateRotation> mousse::coordinateRotation::New
(
  const dictionary& dict
)
{
  if (debug) {
    Pout << "coordinateRotation::New(const dictionary&) : "
      << "constructing coordinateRotation"
      << endl;
  }
  word rotType = dict.lookup("type");
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(rotType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_IO_ERROR_IN
    (
      "coordinateRotation::New"
      "("
      "   const dictionary&, "
      ")",
      dict
    )
    << "Unknown coordinateRotation type "
    << rotType << nl << nl
    << "Valid coordinateRotation types are :" <<  nl
    << "[default: axes ]"
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalIOError);
  }
  return autoPtr<coordinateRotation>{cstrIter()(dict)};
}

