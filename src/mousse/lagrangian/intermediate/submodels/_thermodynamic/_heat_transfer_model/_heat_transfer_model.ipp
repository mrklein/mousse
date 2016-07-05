// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_heat_transfer_model.hpp"


// Constructors 
template<class CloudType>
mousse::HeatTransferModel<CloudType>::HeatTransferModel(CloudType& owner)
:
  CloudSubModelBase<CloudType>{owner},
  BirdCorrection_{false}
{}


template<class CloudType>
mousse::HeatTransferModel<CloudType>::HeatTransferModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type},
  BirdCorrection_{this->coeffDict().lookup("BirdCorrection")}
{}


template<class CloudType>
mousse::HeatTransferModel<CloudType>::HeatTransferModel
(
  const HeatTransferModel<CloudType>& htm
)
:
  CloudSubModelBase<CloudType>{htm},
  BirdCorrection_{htm.BirdCorrection_}
{}


// Destructor 
template<class CloudType>
mousse::HeatTransferModel<CloudType>::~HeatTransferModel()
{}


// Member Functions 
template<class CloudType>
const mousse::Switch& mousse::HeatTransferModel<CloudType>::BirdCorrection() const
{
  return BirdCorrection_;
}


template<class CloudType>
mousse::scalar mousse::HeatTransferModel<CloudType>::htc
(
  const scalar dp,
  const scalar Re,
  const scalar Pr,
  const scalar kappa,
  const scalar NCpW
) const
{
  const scalar Nu = this->Nu(Re, Pr);
  scalar htc = Nu*kappa/dp;
  if (BirdCorrection_ && (mag(htc) > ROOTVSMALL) && (mag(NCpW) > ROOTVSMALL)) {
    const scalar phit = min(NCpW/htc, 50);
    if (phit > 0.001) {
      htc *= phit/(exp(phit) - 1.0);
    }
  }
  return htc;
}

#include "_heat_transfer_model_new.ipp"

