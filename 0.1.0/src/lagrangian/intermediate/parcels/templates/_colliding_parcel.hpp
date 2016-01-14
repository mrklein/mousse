// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CollidingParcel
// Description
//   Wrapper around kinematic parcel types to add collision modelling
// SourceFiles
//   _colliding_parcel_i.hpp
//   _colliding_parcel.cpp
//   _colliding_parcel_io.cpp
#ifndef _colliding_parcel_hpp_
#define _colliding_parcel_hpp_
#include "particle.hpp"
#include "_collision_record_list.hpp"
#include "label_field_io_field.hpp"
#include "vector_field_io_field.hpp"
namespace mousse
{
typedef CollisionRecordList<vector, vector> collisionRecordList;
typedef vectorFieldCompactIOField pairDataFieldCompactIOField;
typedef vectorFieldCompactIOField wallDataFieldCompactIOField;
template<class ParcelType>
class CollidingParcel;
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
    AddToPropertyList
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
      return autoPtr<particle>(new CollidingParcel(*this));
    }
    //- Construct and return a (basic particle) clone
    virtual autoPtr<particle> clone(const polyMesh& mesh) const
    {
      return autoPtr<particle>(new CollidingParcel(*this, mesh));
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
      autoPtr<CollidingParcel<ParcelType> > operator()(Istream& is) const
      {
        return autoPtr<CollidingParcel<ParcelType> >
        (
          new CollidingParcel<ParcelType>(mesh_, is, true)
        );
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
#include "_colliding_parcel_i.hpp"
#ifdef NoRepository
  #include "_colliding_parcel.cpp"
#endif
#endif
