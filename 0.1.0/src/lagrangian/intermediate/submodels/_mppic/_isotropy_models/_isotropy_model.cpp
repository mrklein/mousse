// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_isotropy_model.hpp"
#include "time_scale_model.hpp"
// Constructors 
template<class CloudType>
mousse::IsotropyModel<CloudType>::IsotropyModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>(owner),
  timeScaleModel_(NULL)
{}
template<class CloudType>
mousse::IsotropyModel<CloudType>::IsotropyModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type),
  timeScaleModel_
  (
    TimeScaleModel::New
    (
      this->coeffDict().subDict(TimeScaleModel::typeName)
    )
  )
{}
template<class CloudType>
mousse::IsotropyModel<CloudType>::IsotropyModel
(
  const IsotropyModel<CloudType>& cm
)
:
  CloudSubModelBase<CloudType>(cm),
  timeScaleModel_(cm.timeScaleModel_)
{}
// Destructor 
template<class CloudType>
mousse::IsotropyModel<CloudType>::~IsotropyModel()
{}
//  Selector
template<class CloudType>
mousse::autoPtr<mousse::IsotropyModel<CloudType> >
mousse::IsotropyModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word modelType(dict.lookup(typeName));
  Info<< "Selecting isotropy model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "IsotropyModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )   << "Unknown isotropy model type " << modelType
      << ", constructor not in hash table" << nl << nl
      << "    Valid isotropy model types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return
    autoPtr<IsotropyModel<CloudType> >
    (
      cstrIter()(dict, owner)
    );
}
