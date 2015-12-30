// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cone_injection.hpp"
#include "time_data_entry.hpp"
#include "mathematical_constants.hpp"
#include "unit_conversion.hpp"
using namespace mousse::constant::mathematical;
// Constructors 
template<class CloudType>
mousse::ConeInjection<CloudType>::ConeInjection
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  InjectionModel<CloudType>(dict, owner, modelName, typeName),
  positionAxis_(this->coeffDict().lookup("positionAxis")),
  injectorCells_(positionAxis_.size()),
  injectorTetFaces_(positionAxis_.size()),
  injectorTetPts_(positionAxis_.size()),
  duration_(readScalar(this->coeffDict().lookup("duration"))),
  parcelsPerInjector_
  (
    readScalar(this->coeffDict().lookup("parcelsPerInjector"))
  ),
  flowRateProfile_
  (
    TimeDataEntry<scalar>
    (
      owner.db().time(),
      "flowRateProfile",
      this->coeffDict()
    )
  ),
  Umag_
  (
    TimeDataEntry<scalar>
    (
      owner.db().time(),
      "Umag",
      this->coeffDict()
    )
  ),
  thetaInner_
  (
    TimeDataEntry<scalar>
    (
      owner.db().time(),
      "thetaInner",
      this->coeffDict()
    )
  ),
  thetaOuter_
  (
    TimeDataEntry<scalar>
    (
      owner.db().time(),
      "thetaOuter",
      this->coeffDict()
    )
  ),
  sizeDistribution_
  (
    distributionModels::distributionModel::New
    (
      this->coeffDict().subDict("sizeDistribution"), owner.rndGen()
    )
  ),
  nInjected_(this->parcelsAddedTotal()),
  tanVec1_(positionAxis_.size()),
  tanVec2_(positionAxis_.size())
{
  duration_ = owner.db().time().userTimeToTime(duration_);
  // Normalise direction vector and determine direction vectors
  // tangential to injector axis direction
  forAll(positionAxis_, i)
  {
    vector& axis = positionAxis_[i].second();
    axis /= mag(axis);
    vector tangent = vector::zero;
    scalar magTangent = 0.0;
    cachedRandom& rnd = this->owner().rndGen();
    while (magTangent < SMALL)
    {
      vector v = rnd.sample01<vector>();
      tangent = v - (v & axis)*axis;
      magTangent = mag(tangent);
    }
    tanVec1_[i] = tangent/magTangent;
    tanVec2_[i] = axis^tanVec1_[i];
  }
  // Set total volume to inject
  this->volumeTotal_ = flowRateProfile_.integrate(0.0, duration_);
  updateMesh();
}
template<class CloudType>
mousse::ConeInjection<CloudType>::ConeInjection
(
  const ConeInjection<CloudType>& im
)
:
  InjectionModel<CloudType>(im),
  positionAxis_(im.positionAxis_),
  injectorCells_(im.injectorCells_),
  injectorTetFaces_(im.injectorTetFaces_),
  injectorTetPts_(im.injectorTetPts_),
  duration_(im.duration_),
  parcelsPerInjector_(im.parcelsPerInjector_),
  flowRateProfile_(im.flowRateProfile_),
  Umag_(im.Umag_),
  thetaInner_(im.thetaInner_),
  thetaOuter_(im.thetaOuter_),
  sizeDistribution_(im.sizeDistribution_().clone().ptr()),
  nInjected_(im.nInjected_),
  tanVec1_(im.tanVec1_),
  tanVec2_(im.tanVec2_)
{}
// Destructor 
template<class CloudType>
mousse::ConeInjection<CloudType>::~ConeInjection()
{}
// Member Functions 
template<class CloudType>
void mousse::ConeInjection<CloudType>::updateMesh()
{
  // Set/cache the injector cells
  forAll(positionAxis_, i)
  {
    this->findCellAtPosition
    (
      injectorCells_[i],
      injectorTetFaces_[i],
      injectorTetPts_[i],
      positionAxis_[i].first()
    );
  }
}
template<class CloudType>
mousse::scalar mousse::ConeInjection<CloudType>::timeEnd() const
{
  return this->SOI_ + duration_;
}
template<class CloudType>
mousse::label mousse::ConeInjection<CloudType>::parcelsToInject
(
  const scalar time0,
  const scalar time1
)
{
  if ((time0 >= 0.0) && (time0 < duration_))
  {
    const scalar targetVolume = flowRateProfile_.integrate(0, time1);
    const label targetParcels =
      parcelsPerInjector_*targetVolume/this->volumeTotal_;
    const label nToInject = targetParcels - nInjected_;
    nInjected_ += nToInject;
    return positionAxis_.size()*nToInject;
  }
  else
  {
    return 0;
  }
}
template<class CloudType>
mousse::scalar mousse::ConeInjection<CloudType>::volumeToInject
(
  const scalar time0,
  const scalar time1
)
{
  if ((time0 >= 0.0) && (time0 < duration_))
  {
    return flowRateProfile_.integrate(time0, time1);
  }
  else
  {
    return 0.0;
  }
}
template<class CloudType>
void mousse::ConeInjection<CloudType>::setPositionAndCell
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
  const label i = parcelI % positionAxis_.size();
  position = positionAxis_[i].first();
  cellOwner = injectorCells_[i];
  tetFaceI = injectorTetFaces_[i];
  tetPtI = injectorTetPts_[i];
}
template<class CloudType>
void mousse::ConeInjection<CloudType>::setProperties
(
  const label parcelI,
  const label,
  const scalar time,
  typename CloudType::parcelType& parcel
)
{
  cachedRandom& rnd = this->owner().rndGen();
  // set particle velocity
  const label i = parcelI % positionAxis_.size();
  scalar t = time - this->SOI_;
  scalar ti = thetaInner_.value(t);
  scalar to = thetaOuter_.value(t);
  scalar coneAngle = degToRad(rnd.position<scalar>(ti, to));
  scalar alpha = sin(coneAngle);
  scalar dcorr = cos(coneAngle);
  scalar beta = twoPi*rnd.sample01<scalar>();
  vector normal = alpha*(tanVec1_[i]*cos(beta) + tanVec2_[i]*sin(beta));
  vector dirVec = dcorr*positionAxis_[i].second();
  dirVec += normal;
  dirVec /= mag(dirVec);
  parcel.U() = Umag_.value(t)*dirVec;
  // set particle diameter
  parcel.d() = sizeDistribution_().sample();
}
template<class CloudType>
bool mousse::ConeInjection<CloudType>::fullyDescribed() const
{
  return false;
}
template<class CloudType>
bool mousse::ConeInjection<CloudType>::validInjection(const label)
{
  return true;
}
