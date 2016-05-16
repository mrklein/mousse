// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_eq_model.hpp"


mousse::autoPtr<mousse::XiEqModel> mousse::XiEqModel::New
(
  const dictionary& propDict,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su
)
{
  const word modelType{propDict.lookup("XiEqModel")};
  Info << "Selecting flame-wrinkling model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "XiEqModel::New"
      "("
      "  const psiuReactionThermo& thermo,"
      "  const compressible::RASModel& turbulence,"
      "  const volScalarField& Su"
      ")"
    )
    << "Unknown XiEqModel type "
    << modelType << nl << nl
    << "Valid XiEqModels are : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return {cstrIter()(propDict, thermo, turbulence, Su)};
}

