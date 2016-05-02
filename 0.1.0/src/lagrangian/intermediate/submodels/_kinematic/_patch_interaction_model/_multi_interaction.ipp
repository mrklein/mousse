// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_multi_interaction.hpp"


// Private Member Functions 
template<class CloudType>
bool mousse::MultiInteraction<CloudType>::read(const dictionary& dict)
{
  // Count dictionaries
  Info << "Patch interaction model " << typeName << nl
    << "Executing in turn " << endl;
  label nModels = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter) {
    if (iter().isDict()) {
      Info << "    " << iter().name() << endl;
      nModels++;
    }
  }
  models_.setSize(nModels);
  nModels = 0;
  FOR_ALL_CONST_ITER(dictionary, dict, iter) {
    if (!iter().isDict())
      continue;
    models_.set
    (
      nModels++,
      PatchInteractionModel<CloudType>::New
      (
        iter().dict(),
        this->owner()
      )
    );
  }
  oneInteractionOnly_ = Switch(dict.lookup("oneInteractionOnly"));
  if (oneInteractionOnly_) {
    Info << "Stopping upon first model that interacts with particle."
      << nl << endl;
  } else {
    Info << "Allowing multiple models to interact."
      << nl << endl;
  }
  return true;
}


// Constructors 
template<class CloudType>
mousse::MultiInteraction<CloudType>::MultiInteraction
(
  const dictionary& dict,
  CloudType& cloud
)
:
  PatchInteractionModel<CloudType>{dict, cloud, typeName}
{
  read(this->coeffDict());
}


template<class CloudType>
mousse::MultiInteraction<CloudType>::MultiInteraction
(
  const MultiInteraction<CloudType>& pim
)
:
  PatchInteractionModel<CloudType>{pim},
  oneInteractionOnly_{pim.oneInteractionOnly_},
  models_{pim.models_}
{}


// Destructor 
template<class CloudType>
mousse::MultiInteraction<CloudType>::~MultiInteraction()
{}


// Member Functions 
template<class CloudType>
bool mousse::MultiInteraction<CloudType>::active() const
{
  FOR_ALL(models_, i) {
    if (models_[i].active()) {
      return true;
    }
  }
  return false;
}


template<class CloudType>
bool mousse::MultiInteraction<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const polyPatch& pp,
  bool& keepParticle,
  const scalar trackFraction,
  const tetIndices& tetIs
)
{
  label origFacei = p.face();
  label patchi = pp.index();
  bool interacted = false;
  FOR_ALL(models_, i) {
    bool myInteracted =
      models_[i].correct
      (
        p,
        this->owner().pMesh().boundaryMesh()[patchi],
        keepParticle,
        trackFraction,
        tetIs
      );
    if (myInteracted && oneInteractionOnly_) {
      break;
    }
    interacted = (interacted || myInteracted);
    // Check if perhaps the interaction model has changed patches
    // (CoincidentBaffleInteraction can do this)
    if (p.face() != origFacei) {
      origFacei = p.face();
      patchi = p.patch(p.face());
      // Interaction model has moved particle off wall?
      if (patchi == -1) {
        break;
      }
    }
  }
  return interacted;
}

