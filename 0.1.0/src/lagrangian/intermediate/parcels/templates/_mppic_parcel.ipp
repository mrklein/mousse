// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_mppic_parcel.hpp"


// Constructors 
template<class ParcelType>
mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const MPPICParcel<ParcelType>& p
)
:
  ParcelType{p},
  UCorrect_{p.UCorrect_}
{}


template<class ParcelType>
mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const MPPICParcel<ParcelType>& p,
  const polyMesh& mesh
)
:
  ParcelType{p, mesh},
  UCorrect_{p.UCorrect_}
{}


// Member Functions 
template<class ParcelType>
template<class TrackData>
bool mousse::MPPICParcel<ParcelType>::move
(
  TrackData& td,
  const scalar trackTime
)
{
  typename TrackData::cloudType::parcelType& p =
    static_cast<typename TrackData::cloudType::parcelType&>(*this);
  switch (td.part()) {
    case TrackData::tpLinearTrack:
      {
        ParcelType::move(td, trackTime);
        break;
      }
    case TrackData::tpDampingNoTrack:
      {
        p.UCorrect() =
          td.cloud().dampingModel().velocityCorrection(p, trackTime);
        td.keepParticle = true;
        break;
      }
    case TrackData::tpPackingNoTrack:
      {
        p.UCorrect() =
          td.cloud().packingModel().velocityCorrection(p, trackTime);
        td.keepParticle = true;
        break;
      }
    case TrackData::tpCorrectTrack:
      {
        vector U = p.U();
        scalar f = p.stepFraction();
        scalar a = p.age();
        p.U() = (1.0 - f)*p.UCorrect();
        ParcelType::move(td, trackTime);
        p.U() = U + (p.stepFraction() - f)*p.UCorrect();
        p.age() = a;
        break;
      }
  }
  return td.keepParticle;
}

// IOStream operators 
#include "_mppic_parcel_io.ipp"

