// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_no_inflow.hpp"


// Constructors 
template<class CloudType>
mousse::NoInflow<CloudType>::NoInflow
(
  const dictionary&,
  CloudType& cloud
)
:
  InflowBoundaryModel<CloudType>{cloud}
{}


// Destructor 
template<class CloudType>
mousse::NoInflow<CloudType>::~NoInflow()
{}


// Member Functions 
template<class CloudType>
void mousse::NoInflow<CloudType>::inflow()
{}

