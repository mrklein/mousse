// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_surface_reaction_model.hpp"
// Constructors 
template<class CloudType>
mousse::SurfaceReactionModel<CloudType>::SurfaceReactionModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>(owner),
  dMass_(0.0)
{}
template<class CloudType>
mousse::SurfaceReactionModel<CloudType>::SurfaceReactionModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type),
  dMass_(0.0)
{}
template<class CloudType>
mousse::SurfaceReactionModel<CloudType>::SurfaceReactionModel
(
  const SurfaceReactionModel<CloudType>& srm
)
:
  CloudSubModelBase<CloudType>(srm),
  dMass_(srm.dMass_)
{}
// Destructor 
template<class CloudType>
mousse::SurfaceReactionModel<CloudType>::~SurfaceReactionModel()
{}
// Member Functions 
template<class CloudType>
void mousse::SurfaceReactionModel<CloudType>::addToSurfaceReactionMass
(
  const scalar dMass
)
{
  dMass_ += dMass;
}
template<class CloudType>
void mousse::SurfaceReactionModel<CloudType>::info(Ostream& os)
{
  const scalar mass0 = this->template getBaseProperty<scalar>("mass");
  const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());
  Info<< "    Mass transfer surface reaction  = " << massTotal << nl;
  if (this->outputTime())
  {
    this->setBaseProperty("mass", massTotal);
    dMass_ = 0.0;
  }
}
#include "_surface_reaction_model_new.cpp"
