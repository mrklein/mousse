#ifndef LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TCOLLIDING_PARCEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_PARCELS_TEMPLATES_TCOLLIDING_PARCEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CollidingParcel
// Description
//   Wrapper around kinematic parcel types to add collision modelling

#include "particle.hpp"
#include "_collision_record_list.hpp"
#include "label_field_io_field.hpp"
#include "vector_field_io_field.hpp"


namespace mousse {

typedef CollisionRecordList<vector, vector> collisionRecordList;
typedef vectorFieldCompactIOField pairDataFieldCompactIOField;
typedef vectorFieldCompactIOField wallDataFieldCompactIOField;

template<class ParcelType> class CollidingParcel;

// Forward declaration of friend functions
template<class ParcelType>
Ostream& operator<<
(
  Ostream&,
  const CollidingParcel<ParcelType>&
);


template<class ParcelType>
class CollidingParcel
:
  public ParcelType
{
  // Private member data
    //- Size in bytes of the fields
    static const std::size_t sizeofFields_;
public:
  //- Class to hold thermo particle constant properties
  class constantProperties
  :
    public ParcelType::constantProperties
  {
    // Private data
      //- Young's modulus [N/m2]
      demandDrivenEntry<scalar> youngsModulus_;
      //- Poisson's ratio
      demandDrivenEntry<scalar> poissonsRatio_;
  public:
    // Constructors
      //- Null constructor
      constantProperties();
      //- Copy constructor
      constantProperties(const constantProperties& cp);
      //- Construct from dictionary
      constantProperties(const dictionary& parentDict);
    // Member functions
      //- Return const access to Young's Modulus
      inline scalar youngsModulus() const;
      //- Return const access to Poisson's ratio
      inline scalar poissonsRatio() const;
  };
protected:
  // Protected data
    //- Force on particle due to collisions [N]
    vector f_;
    //- Angular momentum of Parcel in global reference frame [kg m2/s]
    vector angularMomentum_;
    //- Torque on particle due to collisions in global
    //  reference frame [Nm]
    vector torque_;
    //- Particle collision records
    collisionRecordList collisionRecords_;
public:
  // Static data members
    //- Runtime type information
    TYPE_NAME("CollidingParcel");
    //- String representation of properties
    ADD_TO_PROPERTY_LIST
    (
      ParcelType,
      " (fx fy fz)"
      + " (angularMomentumx angularMomentumy angularMomentumz)"
      + " (torquex torquey torquez)"
      + " collisionRecordsPairAccessed"
      + " collisionRecordsPairOrigProcOfOther"
      + " collisionRecordsPairOrigIdOfOther"
      + " (collisionRecordsPairData)"
      + " collisionRecordsWallAccessed"
      + " collisionRecordsWallPRel"
      + " (collisionRecordsWallData)"
    );
  // Constructors
    //- Construct from owner, position, and cloud owner
    //  Other properties initialised as null
    inline CollidingParcel
    (
      const polyMesh& mesh,
      const vector& position,
      const label cellI,
      const label tetFaceI,
      const label tetPtI
    );
    //- Construct from components
    inline CollidingParcel
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
      const vector& f0,
      const vector& angularMomentum0,
      const vector& torque0,
      const typename ParcelType::constantProperties& constProps
    );
    //- Construct from Istream
    CollidingParcel
    (
      const polyMesh& mesh,
      Istream& is,
      bool readFields = true
    );
    //- Construct as a copy
    CollidingParcel(const CollidingParcel& p);
    //- Construct as a copy
    CollidingParcel(const CollidingParcel& p, const polyMesh& mesh);
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone() const
    {
      return autoPtr<particle>{new CollidingParcel{*this}};
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>{new CollidingParcel{*this, mesh}};
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
      autoPtr<CollidingParcel<ParcelType>> operator()(Istream& is) const
      {
        return
          autoPtr<CollidingParcel<ParcelType>>
          {
            new CollidingParcel<ParcelType>{mesh_, is, true}
          };
      }
    };
  // Member Functions
    // Access
      //- Return const access to force
      inline const vector& f() const;
      //- Return const access to angular momentum
      inline const vector& angularMomentum() const;
      //- Return const access to torque
      inline const vector& torque() const;
      //- Return const access to the collision records
      inline const collisionRecordList& collisionRecords() const;
      //- Return access to force
      inline vector& f();
      //- Return access to angular momentum
      inline vector& angularMomentum();
      //- Return access to torque
      inline vector& torque();
      //- Return access to collision records
      inline collisionRecordList& collisionRecords();
      //- Particle angular velocity
      inline vector omega() const;
    // Tracking
      //- Move the parcel
      template<class TrackData>
      bool move(TrackData& td, const scalar trackTime);
      //- Transform the physical properties of the particle
      //  according to the given transformation tensor
      virtual void transformProperties(const tensor& T);
      //- Transform the physical properties of the particle
      //  according to the given separation vector
      virtual void transformProperties(const vector& separation);
   // I-O
      //- Read
      template<class CloudType>
      static void readFields(CloudType& c);
      //- Write
      template<class CloudType>
      static void writeFields(const CloudType& c);
  // Ostream Operator
    friend Ostream& operator<< <ParcelType>
    (
      Ostream&,
      const CollidingParcel<ParcelType>&
    );
};

}  // namespace mousse


// Constructors 
template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::
constantProperties()
:
  ParcelType::constantProperties{},
  youngsModulus_{this->dict_, 0.0},
  poissonsRatio_{this->dict_, 0.0}
{}


template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::constantProperties
(
  const constantProperties& cp
)
:
  ParcelType::constantProperties{cp},
  youngsModulus_{cp.youngsModulus_},
  poissonsRatio_{cp.poissonsRatio_}
{}


template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::constantProperties::constantProperties
(
  const dictionary& parentDict
)
:
  ParcelType::constantProperties{parentDict},
  youngsModulus_{this->dict_, "youngsModulus"},
  poissonsRatio_{this->dict_, "poissonsRatio"}
{}


template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::CollidingParcel
(
  const polyMesh& owner,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  ParcelType{owner, position, cellI, tetFaceI, tetPtI},
  f_{vector::zero},
  angularMomentum_{vector::zero},
  torque_{vector::zero},
  collisionRecords_{}
{}


template<class ParcelType>
inline mousse::CollidingParcel<ParcelType>::CollidingParcel
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
  const vector& /*U0*/,
  const vector& f0,
  const vector& angularMomentum0,
  const vector& torque0,
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
    torque0,
    constProps
  },
  f_{f0},
  angularMomentum_{angularMomentum0},
  torque_{torque0},
  collisionRecords_{}
{}


// constantProperties Member Functions
template<class ParcelType>
inline mousse::scalar
mousse::CollidingParcel<ParcelType>::constantProperties::youngsModulus() const
{
  return youngsModulus_.value();
}


template<class ParcelType>
inline mousse::scalar
mousse::CollidingParcel<ParcelType>::constantProperties::poissonsRatio() const
{
  return poissonsRatio_.value();
}


// CollidingParcel Member Functions 
template<class ParcelType>
inline const mousse::vector& mousse::CollidingParcel<ParcelType>::f() const
{
  return f_;
}


template<class ParcelType>
inline const mousse::vector&
mousse::CollidingParcel<ParcelType>::angularMomentum() const
{
  return angularMomentum_;
}


template<class ParcelType>
inline const mousse::vector& mousse::CollidingParcel<ParcelType>::torque() const
{
  return torque_;
}


template<class ParcelType>
inline const mousse::collisionRecordList&
mousse::CollidingParcel<ParcelType>::collisionRecords() const
{
  return collisionRecords_;
}


template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::f()
{
  return f_;
}


template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::angularMomentum()
{
  return angularMomentum_;
}


template<class ParcelType>
inline mousse::vector& mousse::CollidingParcel<ParcelType>::torque()
{
  return torque_;
}


template<class ParcelType>
inline mousse::collisionRecordList&
mousse::CollidingParcel<ParcelType>::collisionRecords()
{
  return collisionRecords_;
}


template<class ParcelType>
inline mousse::vector mousse::CollidingParcel<ParcelType>::omega() const
{
  return angularMomentum_/this->momentOfInertia();
}

#include "_colliding_parcel.ipp"

#endif