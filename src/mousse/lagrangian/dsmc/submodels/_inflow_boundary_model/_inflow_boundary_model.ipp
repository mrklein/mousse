// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_inflow_boundary_model.hpp"


// Constructors
template<class CloudType>
mousse::InflowBoundaryModel<CloudType>::InflowBoundaryModel(CloudType& owner)
:
  dict_{dictionary::null},
  owner_{owner},
  coeffDict_{dictionary::null}
{}


template<class CloudType>
mousse::InflowBoundaryModel<CloudType>::InflowBoundaryModel
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
mousse::InflowBoundaryModel<CloudType>::~InflowBoundaryModel()
{}


// Member Functions
template<class CloudType>
const CloudType& mousse::InflowBoundaryModel<CloudType>::owner() const
{
  return owner_;
}


template<class CloudType>
CloudType& mousse::InflowBoundaryModel<CloudType>::owner()
{
  return owner_;
}


template<class CloudType>
const mousse::dictionary& mousse::InflowBoundaryModel<CloudType>::dict() const
{
  return dict_;
}


template<class CloudType>
const mousse::dictionary& mousse::InflowBoundaryModel<CloudType>::coeffDict() const
{
  return coeffDict_;
}


#include "_inflow_boundary_model_new.ipp"

