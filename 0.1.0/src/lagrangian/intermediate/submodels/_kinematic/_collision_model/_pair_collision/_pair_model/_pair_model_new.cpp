// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_pair_model.hpp"
template<class CloudType>
mousse::autoPtr<mousse::PairModel<CloudType> >
mousse::PairModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word PairModelType(dict.lookup("pairModel"));
  Info<< "Selecting pair model " << PairModelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(PairModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "PairModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown pair model type "
      << PairModelType
      << ", constructor not in hash table" << nl << nl
      << "    Valid pair model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<PairModel<CloudType> >(cstrIter()(dict, owner));
}
