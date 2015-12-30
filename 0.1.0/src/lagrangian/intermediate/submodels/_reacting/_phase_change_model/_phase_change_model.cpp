// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_phase_change_model.hpp"
// Static Data Members
template<class CloudType>
const mousse::wordList mousse::PhaseChangeModel<CloudType>::
enthalpyTransferTypeNames
(
  IStringStream
  (
    "("
      "latentHeat "
      "enthalpyDifference"
    ")"
  )()
);
// Protected Member Functions 
template<class CloudType>
typename mousse::PhaseChangeModel<CloudType>::enthalpyTransferType
mousse::PhaseChangeModel<CloudType>::wordToEnthalpyTransfer(const word& etName)
const
{
  forAll(enthalpyTransferTypeNames, i)
  {
    if (etName == enthalpyTransferTypeNames[i])
    {
      return enthalpyTransferType(i);
    }
  }
  FatalErrorIn
  (
    "PhaseChangeModel<CloudType>::enthalpyTransferType"
    "PhaseChangeModel<CloudType>::wordToEnthalpyTransfer(const word&) const"
  )   << "Unknown enthalpyType " << etName << ". Valid selections are:" << nl
    << enthalpyTransferTypeNames << exit(FatalError);
  return enthalpyTransferType(0);
}
// Constructors 
template<class CloudType>
mousse::PhaseChangeModel<CloudType>::PhaseChangeModel
(
  CloudType& owner
)
:
  CloudSubModelBase<CloudType>(owner),
  enthalpyTransfer_(etLatentHeat),
  dMass_(0.0)
{}
template<class CloudType>
mousse::PhaseChangeModel<CloudType>::PhaseChangeModel
(
  const PhaseChangeModel<CloudType>& pcm
)
:
  CloudSubModelBase<CloudType>(pcm),
  enthalpyTransfer_(pcm.enthalpyTransfer_),
  dMass_(pcm.dMass_)
{}
template<class CloudType>
mousse::PhaseChangeModel<CloudType>::PhaseChangeModel
(
  const dictionary& dict,
  CloudType& owner,
  const word& type
)
:
  CloudSubModelBase<CloudType>(owner, dict, typeName, type),
  enthalpyTransfer_
  (
    wordToEnthalpyTransfer(this->coeffDict().lookup("enthalpyTransfer"))
  ),
  dMass_(0.0)
{}
// Destructor 
template<class CloudType>
mousse::PhaseChangeModel<CloudType>::~PhaseChangeModel()
{}
// Member Functions 
template<class CloudType>
const typename mousse::PhaseChangeModel<CloudType>::enthalpyTransferType&
mousse::PhaseChangeModel<CloudType>::enthalpyTransfer() const
{
  return enthalpyTransfer_;
}
template<class CloudType>
mousse::scalar mousse::PhaseChangeModel<CloudType>::dh
(
  const label idc,
  const label idl,
  const scalar p,
  const scalar T
) const
{
  return 0.0;
}
template<class CloudType>
mousse::scalar mousse::PhaseChangeModel<CloudType>::TMax
(
  const scalar p,
  const scalarField& X
) const
{
  return GREAT;
}
template<class CloudType>
mousse::scalar mousse::PhaseChangeModel<CloudType>::Tvap(const scalarField& X) const
{
  return -GREAT;
}
template<class CloudType>
void mousse::PhaseChangeModel<CloudType>::addToPhaseChangeMass(const scalar dMass)
{
  dMass_ += dMass;
}
template<class CloudType>
void mousse::PhaseChangeModel<CloudType>::info(Ostream& os)
{
  const scalar mass0 = this->template getBaseProperty<scalar>("mass");
  const scalar massTotal = mass0 + returnReduce(dMass_, sumOp<scalar>());
  Info<< "    Mass transfer phase change      = " << massTotal << nl;
  if (this->outputTime())
  {
    this->setBaseProperty("mass", massTotal);
    dMass_ = 0.0;
  }
}
#include "_phase_change_model_new.cpp"
