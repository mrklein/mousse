// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_devolatilisation_model.hpp"
// Constructors
template<class CloudType>
mousse::DevolatilisationModel<CloudType>::DevolatilisationModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>{owner},
  dMass_{0.0}
{}
template<class CloudType>
mousse::DevolatilisationModel<CloudType>::DevolatilisationModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>{owner, dict, typeName, type},
  dMass_{0.0}
{}
template<class CloudType>
mousse::DevolatilisationModel<CloudType>::DevolatilisationModel
(
  const DevolatilisationModel<CloudType>& dm
)
:
  CloudSubModelBase<CloudType>{dm},
  dMass_{dm.dMass_}
{}
// Destructor
template<class CloudType>
mousse::DevolatilisationModel<CloudType>::~DevolatilisationModel()
{}
// Member Functions
template<class CloudType>
void mousse::DevolatilisationModel<CloudType>::addToDevolatilisationMass
(
  const scalar dMass
)
{
  dMass_ += dMass;
}
template<class CloudType>
void mousse::DevolatilisationModel<CloudType>::info(Ostream&)
{
  const scalar mass0 = this->template getBaseProperty<scalar>("mass");
  const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());
  Info<< "    Mass transfer devolatilisation  = " << massTotal << nl;
  if (this->outputTime())
  {
    this->setBaseProperty("mass", massTotal);
    dMass_ = 0.0;
  }
}
#include "_devolatilisation_model_new.cpp"
