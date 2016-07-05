// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_packing_model.hpp"
#include "_averaging_method.hpp"
#include "particle_stress_model.hpp"
#include "correction_limiting_method.hpp"


// Constructors
template<class CloudType>
mousse::PackingModel<CloudType>::PackingModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>{owner},
  particleStressModel_{nullptr}
{}


template<class CloudType>
mousse::PackingModel<CloudType>::PackingModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type},
  particleStressModel_
  {
    ParticleStressModel::New
    (
      this->coeffDict().subDict(ParticleStressModel::typeName)
    )
  }
{}


template<class CloudType>
mousse::PackingModel<CloudType>::PackingModel(const PackingModel<CloudType>& cm)
:
  CloudSubModelBase<CloudType>{cm},
  particleStressModel_{cm.particleStressModel_}
{}


// Destructor
template<class CloudType>
mousse::PackingModel<CloudType>::~PackingModel()
{}


// Selector
template<class CloudType>
mousse::autoPtr<mousse::PackingModel<CloudType> >
mousse::PackingModel<CloudType>::New
(
  const dictionary& dict,
  CloudType& owner
)
{
  word modelType{dict.lookup(typeName)};
  Info << "Selecting packing model " << modelType << endl;
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(modelType);
  if (cstrIter == dictionaryConstructorTablePtr_->end()) {
    FATAL_ERROR_IN
    (
      "PackingModel<CloudType>::New"
      "("
        "const dictionary&, "
        "CloudType&"
      ")"
    )
    << "Unknown packing model type " << modelType
    << ", constructor not in hash table" << nl << nl
    << "    Valid packing model types are:" << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<PackingModel<CloudType>>{cstrIter()(dict, owner)};
}

