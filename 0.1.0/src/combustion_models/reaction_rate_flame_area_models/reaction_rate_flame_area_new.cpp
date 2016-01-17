// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reaction_rate_flame_area.hpp"
mousse::autoPtr<mousse::reactionRateFlameArea> mousse::reactionRateFlameArea::New
(
  const dictionary& dict,
  const fvMesh& mesh,
  const combustionModel& combModel
)
{
  word reactionRateFlameAreaType
  (
    dict.lookup("reactionRateFlameArea")
  );
  Info<< "Selecting reaction rate flame area correlation "
    << reactionRateFlameAreaType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(reactionRateFlameAreaType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_IO_ERROR_IN
    (
      "reactionRateFlameArea::New(const psiReactionThermo&)",
      dict
    )   << "Unknown reactionRateFlameArea type "
      << reactionRateFlameAreaType << endl << endl
      << "Valid reaction rate flame area types are :" << endl
      << dictionaryConstructorTablePtr_->toc()
      << exit(FatalIOError);
  }
  const label tempOpen = reactionRateFlameAreaType.find('<');
  const word className = reactionRateFlameAreaType(0, tempOpen);
  return autoPtr<reactionRateFlameArea>
    (cstrIter()(className, dict, mesh, combModel));
}
