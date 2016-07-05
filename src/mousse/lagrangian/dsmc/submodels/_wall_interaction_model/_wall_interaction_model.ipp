// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_interaction_model.hpp"


// Constructors 
template<class CloudType>
mousse::WallInteractionModel<CloudType>::WallInteractionModel(CloudType& owner)
:
  dict_{dictionary::null},
  owner_{owner},
  coeffDict_{dictionary::null}
{}


template<class CloudType>
mousse::WallInteractionModel<CloudType>::WallInteractionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  dict_{dict},
  owner_{owner},
  coeffDict_{dict.subDict(type + "Coeffs")}
{}


// Destructor 
template<class CloudType>
mousse::WallInteractionModel<CloudType>::~WallInteractionModel()
{}


// Member Functions 
template<class CloudType>
const CloudType& mousse::WallInteractionModel<CloudType>::owner() const
{
  return owner_;
}


template<class CloudType>
CloudType& mousse::WallInteractionModel<CloudType>::owner()
{
  return owner_;
}


template<class CloudType>
const mousse::dictionary& mousse::WallInteractionModel<CloudType>::dict() const
{
  return dict_;
}


template<class CloudType>
const mousse::dictionary&
mousse::WallInteractionModel<CloudType>::coeffDict() const
{
  return coeffDict_;
}

#include "_wall_interaction_model_new.ipp"

