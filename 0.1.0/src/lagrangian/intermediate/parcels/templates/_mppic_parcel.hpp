// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MPPICParcel
// Description
//   Wrapper around kinematic parcel types to add MPPIC modelling
// SourceFiles
//   _mppic_parcel_i.hpp
//   _mppic_parcel_tracking_data_i.hpp
//   _mppic_parcel.cpp
//   _mppic_parcel_io.cpp
#ifndef _mppic_parcel_hpp_
#define _mppic_parcel_hpp_
#include "particle.hpp"
#include "label_field_io_field.hpp"
#include "vector_field_io_field.hpp"
namespace mousse
{
// Forward declaration of clases
template<class ParcelType>
class MPPICParcel;
template<class Type>
class AveragingMethod;
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
        autoPtr<AveragingMethod<scalar> > volumeAverage_;
        //- Radius average [ volume^(1/3) ]
        autoPtr<AveragingMethod<scalar> > radiusAverage_;
        //- Density average
        autoPtr<AveragingMethod<scalar> > rhoAverage_;
        //- Velocity average
        autoPtr<AveragingMethod<vector> > uAverage_;
        //- Magnitude velocity sqyuared average
        autoPtr<AveragingMethod<scalar> > uSqrAverage_;
        //- Frequency average
        autoPtr<AveragingMethod<scalar> > frequencyAverage_;
        //- Mass average
        autoPtr<AveragingMethod<scalar> > massAverage_;
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
    AddToPropertyList
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
      return autoPtr<particle>(new MPPICParcel(*this));
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>(new MPPICParcel(*this, mesh));
    }
    //- Factory class to read-construct particles used for
    //  parallel transfer
    class iNew
    {
      const polyMesh& mesh_;
    public:
      iNew(const polyMesh& mesh)
      :
        mesh_(mesh)
      {}
      autoPtr<MPPICParcel<ParcelType> > operator()(Istream& is) const
      {
        return autoPtr<MPPICParcel<ParcelType> >
        (
          new MPPICParcel<ParcelType>(mesh_, is, true)
        );
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
#include "_mppic_parcel_i.hpp"
#include "_mppic_parcel_tracking_data_i.hpp"
#ifdef NoRepository
  #include "_mppic_parcel.cpp"
#endif
#endif
