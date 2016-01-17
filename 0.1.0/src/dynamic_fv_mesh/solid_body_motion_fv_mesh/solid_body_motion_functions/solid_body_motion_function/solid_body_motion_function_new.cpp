// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_body_motion_function.hpp"
// Selectors
mousse::autoPtr<mousse::solidBodyMotionFunction> mousse::solidBodyMotionFunction::New
(
  const dictionary& SBMFCoeffs,
  const Time& runTime
)
{
  const word motionType(SBMFCoeffs.lookup("solidBodyMotionFunction"));
  Info<< "Selecting solid-body motion function " << motionType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(motionType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "solidBodyMotionFunction::New"
      "("
      "    const dictionary& SBMFCoeffs,"
      "    const Time& runTime"
      ")"
    )   << "Unknown solidBodyMotionFunction type "
      << motionType << nl << nl
      << "Valid solidBodyMotionFunctions are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<solidBodyMotionFunction>(cstrIter()(SBMFCoeffs, runTime));
}
