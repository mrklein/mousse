// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_g_model.hpp"
mousse::autoPtr<mousse::XiGModel> mousse::XiGModel::New
(
  const dictionary& propDict,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
{
  const word modelType{propDict.lookup("XiGModel")};
  Info<< "Selecting flame-wrinkling model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "XiGModel::New"
      "("
      "    const psiuReactionThermo& thermo,"
      "    const compressible::RASModel& turbulence,"
      "    const volScalarField& Su"
      ")"
    )
    << "Unknown XiGModel type "
    << modelType << nl << nl
    << "Valid XiGModels are : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return {cstrIter()(propDict, thermo, turbulence, Su)};
}
