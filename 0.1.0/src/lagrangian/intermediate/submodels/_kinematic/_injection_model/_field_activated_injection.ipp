// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_field_activated_injection.hpp"
#include "vol_fields.hpp"
#include "mathematical_constants.hpp"


using namespace mousse::constant::mathematical;


// Constructors 
template<class CloudType>
mousse::FieldActivatedInjection<CloudType>::FieldActivatedInjection
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  InjectionModel<CloudType>{dict, owner, modelName, typeName},
  factor_{readScalar(this->coeffDict().lookup("factor"))},
  referenceField_
  {
    owner.db().objectRegistry::template lookupObject<volScalarField>
    (
      this->coeffDict().lookup("referenceField")
    )
  },
  thresholdField_
  {
    owner.db().objectRegistry::template lookupObject<volScalarField>
    (
      this->coeffDict().lookup("thresholdField")
    )
  },
  positionsFile_{this->coeffDict().lookup("positionsFile")},
  positions_
  {
    {
      positionsFile_,
      owner.db().time().constant(),
      owner.mesh(),
      IOobject::MUST_READ,
      IOobject::NO_WRITE
    }
  },
  injectorCells_{positions_.size()},
  injectorTetFaces_{positions_.size()},
  injectorTetPts_{positions_.size()},
  nParcelsPerInjector_
  {
    readLabel(this->coeffDict().lookup("parcelsPerInjector"))
  },
  nParcelsInjected_{positions_.size(), 0},
  U0_{this->coeffDict().lookup("U0")},
  diameters_{positions_.size()},
  sizeDistribution_
  {
    distributionModels::distributionModel::New
    (
      this->coeffDict().subDict("sizeDistribution"),
      owner.rndGen()
    )
  }
{
  // Construct parcel diameters - one per injector cell
  FOR_ALL(diameters_, i) {
    diameters_[i] = sizeDistribution_->sample();
  }
  // Determine total volume of particles to inject
  this->volumeTotal_ =
    nParcelsPerInjector_*sum(pow3(diameters_))*pi/6.0;
  updateMesh();
}


template<class CloudType>
mousse::FieldActivatedInjection<CloudType>::FieldActivatedInjection
(
  const FieldActivatedInjection<CloudType>& im
)
:
  InjectionModel<CloudType>{im},
  factor_{im.factor_},
  referenceField_{im.referenceField_},
  thresholdField_{im.thresholdField_},
  positionsFile_{im.positionsFile_},
  positions_{im.positions_},
  injectorCells_{im.injectorCells_},
  injectorTetFaces_{im.injectorTetFaces_},
  injectorTetPts_{im.injectorTetPts_},
  nParcelsPerInjector_{im.nParcelsPerInjector_},
  nParcelsInjected_{im.nParcelsInjected_},
  U0_{im.U0_},
  diameters_{im.diameters_},
  sizeDistribution_{im.sizeDistribution_().clone().ptr()}
{}


// Destructor 
template<class CloudType>
mousse::FieldActivatedInjection<CloudType>::~FieldActivatedInjection()
{}


// Member Functions 
template<class CloudType>
void mousse::FieldActivatedInjection<CloudType>::updateMesh()
{
  // Set/cache the injector cells
  FOR_ALL(positions_, i) {
    this->findCellAtPosition
    (
      injectorCells_[i],
      injectorTetFaces_[i],
      injectorTetPts_[i],
      positions_[i]
    );
  }
}


template<class CloudType>
mousse::scalar mousse::FieldActivatedInjection<CloudType>::timeEnd() const
{
  return GREAT;
}


template<class CloudType>
mousse::label mousse::FieldActivatedInjection<CloudType>::parcelsToInject
(
  const scalar /*time0*/,
  const scalar /*time1*/
)
{
  if (sum(nParcelsInjected_) < nParcelsPerInjector_*positions_.size()) {
    return positions_.size();
  } else {
    return 0;
  }
}


template<class CloudType>
mousse::scalar mousse::FieldActivatedInjection<CloudType>::volumeToInject
(
  const scalar /*time0*/,
  const scalar /*time1*/
)
{
  if (sum(nParcelsInjected_) < nParcelsPerInjector_*positions_.size()) {
    return this->volumeTotal_/nParcelsPerInjector_;
  } else {
    return 0;
  }
}


template<class CloudType>
void mousse::FieldActivatedInjection<CloudType>::setPositionAndCell
(
  const label parcelI,
  const label,
  const scalar,
  vector& position,
  label& cellOwner,
  label& tetFaceI,
  label& tetPtI
)
{
  position = positions_[parcelI];
  cellOwner = injectorCells_[parcelI];
  tetFaceI = injectorTetFaces_[parcelI];
  tetPtI = injectorTetPts_[parcelI];
}


template<class CloudType>
void mousse::FieldActivatedInjection<CloudType>::setProperties
(
  const label parcelI,
  const label,
  const scalar,
  typename CloudType::parcelType& parcel
)
{
  // set particle velocity
  parcel.U() = U0_;
  // set particle diameter
  parcel.d() = diameters_[parcelI];
}


template<class CloudType>
bool mousse::FieldActivatedInjection<CloudType>::fullyDescribed() const
{
  return false;
}


template<class CloudType>
bool mousse::FieldActivatedInjection<CloudType>::validInjection
(
  const label parcelI
)
{
  const label cellI = injectorCells_[parcelI];
  if (nParcelsInjected_[parcelI] < nParcelsPerInjector_
      && factor_*referenceField_[cellI] > thresholdField_[cellI]) {
    nParcelsInjected_[parcelI]++;
    return true;
  }
  return false;
}

