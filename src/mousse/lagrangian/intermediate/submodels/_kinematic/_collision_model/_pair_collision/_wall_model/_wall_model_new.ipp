// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_model.hpp"


template<class CloudType>
mousse::autoPtr<mousse::WallModel<CloudType> >
mousse::WallModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word WallModelType{dict.lookup("wallModel")};
  Info << "Selecting wall model " << WallModelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(WallModelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "WallModel<CloudType>::New"
      "("
      "  const dictionary&, "
      "  CloudType&"
      ")"
    )
    << "Unknown wall model type type " << WallModelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid wall model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc() << exit(FatalError);
  }
  return autoPtr<WallModel<CloudType>>{cstrIter()(dict, owner)};
}

