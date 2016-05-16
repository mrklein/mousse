// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_inflow_boundary_model.hpp"


template<class CloudType>
mousse::autoPtr<mousse::InflowBoundaryModel<CloudType>>
mousse::InflowBoundaryModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  const word modelType{dict.lookup("InflowBoundaryModel")};
  Info << "Selecting InflowBoundaryModel " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "InflowBoundaryModel<CloudType>::New"
      "(const dictionary&, CloudType&)"
    )
    << "Unknown InflowBoundaryModel type "
    << modelType << nl << nl
    << "Valid InflowBoundaryModel types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<InflowBoundaryModel<CloudType>>{cstrIter()(dict, owner)};
}

