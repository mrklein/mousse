// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_colliding_parcel.hpp"


// Constructors 
template<class ParcelType>
mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const CollidingParcel<ParcelType>& p
)
:
  ParcelType{p},
  f_{p.f_},
  angularMomentum_{p.angularMomentum_},
  torque_{p.torque_},
  collisionRecords_{p.collisionRecords_}
{}


template<class ParcelType>
mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const CollidingParcel<ParcelType>& p,
  const polyMesh& mesh
)
:
  ParcelType{p, mesh},
  f_{p.f_},
  angularMomentum_{p.angularMomentum_},
  torque_{p.torque_},
  collisionRecords_{p.collisionRecords_}
{}


// Member Functions 
template<class ParcelType>
template<class TrackData>
bool mousse::CollidingParcel<ParcelType>::move
(
  TrackData& td,
  const scalar trackTime
)
{
  typename TrackData::cloudType::parcelType& p =
    static_cast<typename TrackData::cloudType::parcelType&>(*this);
  switch (td.part()) {
    case TrackData::tpVelocityHalfStep:
      {
        // First and last leapfrog velocity adjust part, required
        // before and after tracking and force calculation
        p.U() += 0.5*trackTime*p.f()/p.mass();
        p.angularMomentum() += 0.5*trackTime*p.torque();
        td.keepParticle = true;
        break;
      }
    case TrackData::tpLinearTrack:
      {
        ParcelType::move(td, trackTime);
        break;
      }
    case TrackData::tpRotationalTrack:
      {
        NOT_IMPLEMENTED("TrackData::tpRotationalTrack");
        break;
      }
    default:
      {
        FATAL_ERROR_IN
          (
            "CollidingParcel<ParcelType>::move(TrackData&, const scalar)"
          )
          << td.part() << " is an invalid part of the tracking method."
          << abort(FatalError);
      }
  }
  return td.keepParticle;
}


template<class ParcelType>
void mousse::CollidingParcel<ParcelType>::transformProperties(const tensor& T)
{
  ParcelType::transformProperties(T);
  f_ = transform(T, f_);
  angularMomentum_ = transform(T, angularMomentum_);
  torque_ = transform(T, torque_);
}


template<class ParcelType>
void mousse::CollidingParcel<ParcelType>::transformProperties
(
  const vector& separation
)
{
  ParcelType::transformProperties(separation);
}

// IOStream operators 
#include "_colliding_parcel_io.ipp"
