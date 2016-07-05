#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TMPPIC_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TMPPIC_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MPPICParcel
// Description
//   Wrapper around kinematic parcel types to add MPPIC modelling

#include "particle.hpp"
#include "label_field_io_field.hpp"
#include "vector_field_io_field.hpp"
#include "_averaging_method.hpp"


namespace mousse {

// Forward declaration of clases
template<class ParcelType> class MPPICParcel;
template<class Type> class AveragingMethod;

// Forward declaration of friend functions
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const MPPICParcel<ParcelType>&
);


template<class ParcelType>
class MPPICParcel
:
  public ParcelType
{
  // Private data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  template<class CloudType>
  class TrackingData
  :
    public ParcelType::template TrackingData<CloudType>
  {
  public:
    enum trackPart
    {
      tpLinearTrack,
      tpDampingNoTrack,
      tpPackingNoTrack,
      tpCorrectTrack,
    };
  private:
    // Private data
      // MPPIC Averages
        //- Volume average
        autoPtr<AveragingMethod<scalar>> volumeAverage_;
        //- Radius average [ volume^(1/3) ]
        autoPtr<AveragingMethod<scalar>> radiusAverage_;
        //- Density average
        autoPtr<AveragingMethod<scalar>> rhoAverage_;
        //- Velocity average
        autoPtr<AveragingMethod<vector>> uAverage_;
        //- Magnitude velocity sqyuared average
        autoPtr<AveragingMethod<scalar>> uSqrAverage_;
        //- Frequency average
        autoPtr<AveragingMethod<scalar>> frequencyAverage_;
        //- Mass average
        autoPtr<AveragingMethod<scalar>> massAverage_;
      //- Label specifying the current part of the tracking process
      trackPart part_;
  public:
    //- Constructors
      //- Construct from components
      inline TrackingData
      (
        CloudType& cloud,
        trackPart part = tpLinearTrack
      );
    //- Update the MPPIC averages
    inline void updateAverages(CloudType& cloud);
    //- Access
      //- Const access to the tracking part label
      inline trackPart part() const;
      //- Non const access to the tracking part label
      inline trackPart& part();
  };
protected:
  // Protected data
    //- Velocity correction due to collisions [m/s]
    vector UCorrect_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("MPPICParcel");
    //- String representation of properties
    ADD_TO_PROPERTY_LIST
    (
      ParcelType,
      "(UCorrectx UCorrecty UCorrectz)"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline MPPICParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline MPPICParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI,
      const label typeId,
      const scalar nParticle0,
      const scalar d0,
      const scalar dTarget0,
      const vector& U0,
      const vector& UCorrect0,
      const typename ParcelType::constantProperties& constProps
    );
    //- Construct from Istream
    MPPICParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    MPPICParcel(const MPPICParcel& p);
    //- Construct as a copy
    MPPICParcel(const MPPICParcel& p, const polyMesh& mesh);
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new MPPICParcel{*this}};
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>{new MPPICParcel{*this, mesh}};
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_{mesh}
      {}
      autoPtr<MPPICParcel<ParcelType>> operator()(Istream& is) const
      {
        return autoPtr<MPPICParcel<ParcelType>>
        {
          new MPPICParcel<ParcelType>{mesh_, is, true}
        };
      }
    };
  // Member Functions
    // Access
      //- Return const access to correction velocity
      inline const vector& UCorrect() const;
      //- Return access to correction velocity
      inline vector& UCorrect();
    // Tracking
      //- Move the parcel
      template<class TrackData>
      bool move(TrackData& td, const scalar trackTime);
  // Friend Functions
    // I-O
      //- Read
      template<class CloudType>
      static void readFields(CloudType& c);
      //- Write
      template<class CloudType>
      static void writeFields(const CloudType& c);
    // Ostream operator
      friend Ostream& operator<< <ParcelType>
      (
        Ostream&,
        const MPPICParcel<ParcelType>&
      );
};

}  // namespace mousse


// Constructors 
template<class ParcelType>
inline mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType{owner, position, cellI, tetFaceI, tetPtI},
  UCorrect_{vector::zero}
{}


template<class ParcelType>
inline mousse::MPPICParcel<ParcelType>::MPPICParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label typeId,
  const scalar nParticle0,
  const scalar d0,
  const scalar dTarget0,
  const vector& U0,
  const vector& UCorrect0,
  const typename ParcelType::constantProperties& constProps
)
:
  ParcelType
  {
    owner,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    typeId,
    nParticle0,
    d0,
    dTarget0,
    U0,
    constProps
  },
  UCorrect_{UCorrect0}
{}


// Member Functions 
template<class ParcelType>
inline const mousse::vector& mousse::MPPICParcel<ParcelType>::UCorrect() const
{
  return UCorrect_;
}


template<class ParcelType>
inline mousse::vector& mousse::MPPICParcel<ParcelType>::UCorrect()
{
  return UCorrect_;
}



template<class ParcelType>
template<class CloudType>
inline mousse::MPPICParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
  CloudType& cloud,
  trackPart part
)
:
  ParcelType::template TrackingData<CloudType>{cloud},
  volumeAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":volumeAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  radiusAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":radiusAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  rhoAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":rhoAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  uAverage_
  {
    AveragingMethod<vector>::New
    (
      // IOobject
      {
        cloud.name() + ":uAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  uSqrAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":uSqrAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  frequencyAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":frequencyAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  massAverage_
  {
    AveragingMethod<scalar>::New
    (
      // IOobject
      {
        cloud.name() + ":massAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  },
  part_(part)
{}


template<class ParcelType>
template<class CloudType>
inline void
mousse::MPPICParcel<ParcelType>::TrackingData<CloudType>::updateAverages
(
  CloudType& cloud
)
{
  // zero the sums
  volumeAverage_() = 0;
  radiusAverage_() = 0;
  rhoAverage_() = 0;
  uAverage_() = vector::zero;
  uSqrAverage_() = 0;
  frequencyAverage_() = 0;
  massAverage_() = 0;
  // temporary weights
  autoPtr<AveragingMethod<scalar>> weightAveragePtr
  {
    AveragingMethod<scalar>::New
    (
      {
        cloud.name() + ":weightAverage",
        cloud.db().time().timeName(),
        cloud.mesh()
      },
      cloud.solution().dict(),
      cloud.mesh()
    )
  };
  AveragingMethod<scalar>& weightAverage = weightAveragePtr();
  // averaging sums
  FOR_ALL_CONST_ITER(typename CloudType, cloud, iter) {
    const typename CloudType::parcelType& p = iter();
    const tetIndices tetIs(p.cell(), p.tetFace(), p.tetPt(), cloud.mesh());
    const scalar m = p.nParticle()*p.mass();
    volumeAverage_->add(p.position(), tetIs, p.nParticle()*p.volume());
    rhoAverage_->add(p.position(), tetIs, m*p.rho());
    uAverage_->add(p.position(), tetIs, m*p.U());
    massAverage_->add(p.position(), tetIs, m);
  }
  volumeAverage_->average();
  massAverage_->average();
  rhoAverage_->average(massAverage_);
  uAverage_->average(massAverage_);
  // squared velocity deviation
  FOR_ALL_CONST_ITER(typename CloudType, cloud, iter) {
    const typename CloudType::parcelType& p = iter();
    const tetIndices tetIs(p.cell(), p.tetFace(), p.tetPt(), cloud.mesh());
    const vector u = uAverage_->interpolate(p.position(), tetIs);
    uSqrAverage_->add
    (
      p.position(),
      tetIs,
      p.nParticle()*p.mass()*magSqr(p.U() - u)
    );
  }
  uSqrAverage_->average(massAverage_);
  // sauter mean radius
  radiusAverage_() = volumeAverage_();
  weightAverage = 0;
  FOR_ALL_CONST_ITER(typename CloudType, cloud, iter) {
    const typename CloudType::parcelType& p = iter();
    const tetIndices tetIs{p.cell(), p.tetFace(), p.tetPt(), cloud.mesh()};
    weightAverage.add
    (
      p.position(),
      tetIs,
      p.nParticle()*pow(p.volume(), 2.0/3.0)
    );
  }
  weightAverage.average();
  radiusAverage_->average(weightAverage);
  // collision frequency
  weightAverage = 0;
  FOR_ALL_CONST_ITER(typename CloudType, cloud, iter) {
    const typename CloudType::parcelType& p = iter();
    tetIndices tetIs{p.cell(), p.tetFace(), p.tetPt(), cloud.mesh()};
    const scalar a = volumeAverage_->interpolate(p.position(), tetIs);
    const scalar r = radiusAverage_->interpolate(p.position(), tetIs);
    const vector u = uAverage_->interpolate(p.position(), tetIs);
    const scalar f = 0.75*a/pow3(r)*sqr(0.5*p.d() + r)*mag(p.U() - u);
    frequencyAverage_->add(p.position(), tetIs, p.nParticle()*f*f);
    weightAverage.add(p.position(), tetIs, p.nParticle()*f);
  }
  frequencyAverage_->average(weightAverage);
}


template<class ParcelType>
template<class CloudType>
inline typename mousse::MPPICParcel<ParcelType>::template
TrackingData<CloudType>::trackPart
mousse::MPPICParcel<ParcelType>::TrackingData<CloudType>::part() const
{
  return part_;
}


template<class ParcelType>
template<class CloudType>
inline typename mousse::MPPICParcel<ParcelType>::template
TrackingData<CloudType>::trackPart&
mousse::MPPICParcel<ParcelType>::TrackingData<CloudType>::part()
{
  return part_;
}

#include "_mppic_parcel.ipp"

#endif
