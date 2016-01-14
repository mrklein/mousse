// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_heat_transfer_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::HeatTransferModel<CloudType> >
mousse::HeatTransferModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType(dict.lookup("heatTransferModel"));
  Info<< "Selecting heat transfer model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "HeatTransferModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown heat transfer model type "
      << modelType << nl << nl
      << "Valid heat transfer model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<HeatTransferModel<CloudType> >(cstrIter()(dict, owner));
}
