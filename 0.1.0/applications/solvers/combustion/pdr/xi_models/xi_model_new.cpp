// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "xi_model.hpp"


mousse::autoPtr<mousse::XiModel> mousse::XiModel::New
(
  const dictionary& propDict,
  const psiuReactionThermo& thermo,
  const compressible::RASModel& turbulence,
  const volScalarField& Su,
  const volScalarField& rho,
  const volScalarField& b,
  const surfaceScalarField& phi
)
{
  const word modelType{propDict.lookup("XiModel")};
  Info << "Selecting flame-wrinkling model " << modelType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "XiModel::New"
    )
    << "Unknown XiModel type "
    << modelType << nl << nl
    << "Valid XiModels are : " << endl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return
    autoPtr<XiModel>
    {
      cstrIter()(propDict, thermo, turbulence, Su, rho, b, phi)
    };
}
