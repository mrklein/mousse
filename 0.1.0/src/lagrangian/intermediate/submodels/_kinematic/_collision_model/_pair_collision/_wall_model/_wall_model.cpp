// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wall_model.hpp"
// Constructors 
template<class CloudType>
mousse::WallModel<CloudType>::WallModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  dict_(dict),
  owner_(owner),
  coeffDict_(dict.subDict(type + "Coeffs"))
{}
// Destructor 
template<class CloudType>
mousse::WallModel<CloudType>::~WallModel()
{}
// Member Functions 
template<class CloudType>
const CloudType&
mousse::WallModel<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
CloudType&
mousse::WallModel<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
const mousse::dictionary& mousse::WallModel<CloudType>::dict() const
{
  return dict_;
}
template<class CloudType>
const mousse::dictionary&
mousse::WallModel<CloudType>::coeffDict() const
{
  return coeffDict_;
}
#include "_wall_model_new.cpp"
