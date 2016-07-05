// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ranz_marshall.hpp"


// Constructors 
template<class CloudType>
mousse::RanzMarshall<CloudType>::RanzMarshall
(
  const dictionary& dict,
  CloudType& cloud
)
:
  HeatTransferModel<CloudType>{dict, cloud, typeName}
{}


template<class CloudType>
mousse::RanzMarshall<CloudType>::RanzMarshall(const RanzMarshall<CloudType>& htm)
:
  HeatTransferModel<CloudType>{htm}
{}


// Destructor 
template<class CloudType>
mousse::RanzMarshall<CloudType>::~RanzMarshall()
{}


// Member Functions 
template<class CloudType>
mousse::scalar mousse::RanzMarshall<CloudType>::Nu
(
  const scalar Re,
  const scalar Pr
) const
{
  return 2.0 + 0.6*sqrt(Re)*cbrt(Pr);
}

