// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_breakup_model.hpp"


template<class CloudType>
mousse::autoPtr<mousse::BreakupModel<CloudType>>
mousse::BreakupModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word BreakupModelType{dict.lookup("breakupModel")};
  Info << "Selecting BreakupModel " << BreakupModelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(BreakupModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "BreakupModel<CloudType>::New"
      "("
      "  const dictionary&, "
      "  CloudType&"
      ")"
    )
    << "Unknown BreakupModelType type "
    << BreakupModelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid BreakupModel types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<BreakupModel<CloudType>>{cstrIter()(dict, owner)};
}
