// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_pair_model.hpp"
// Constructors 
template<class CloudType>
mousse::PairModel<CloudType>::PairModel
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
mousse::PairModel<CloudType>::~PairModel()
{}
// Member Functions 
template<class CloudType>
const CloudType&
mousse::PairModel<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
const mousse::dictionary& mousse::PairModel<CloudType>::dict() const
{
  return dict_;
}
template<class CloudType>
const mousse::dictionary&
mousse::PairModel<CloudType>::coeffDict() const
{
  return coeffDict_;
}
#include "_pair_model_new.cpp"
