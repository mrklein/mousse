// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CollidingCloud
// Description
//   Adds coolisions to kinematic clouds
// SourceFiles
//   _colliding_cloud_i.hpp
//   _colliding_cloud.cpp
#ifndef _colliding_cloud_hpp_
#define _colliding_cloud_hpp_
#include "particle.hpp"
#include "_cloud.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
namespace mousse
{
// Forward declaration of classes
template<class CloudType>
class CollisionModel;
template<class CloudType>
class CollidingCloud
:
  public CloudType
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef CollidingCloud<CloudType> collidingCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<CollidingCloud<CloudType> > cloudCopyPtr_;
  // Private Member Functions
    //- Disallow default bitwise copy construct
    CollidingCloud(const CollidingCloud&);
    //- Disallow default bitwise assignment
    void operator=(const CollidingCloud&);
protected:
  // Protected data
    //- Thermo parcel constant properties
    typename parcelType::constantProperties constProps_;
    // References to the cloud sub-models
      //- Collision model
      autoPtr<CollisionModel<CollidingCloud<CloudType> > >
        collisionModel_;
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Move-collide particles
      template<class TrackData>
      void moveCollide(TrackData& td, const scalar deltaT);
      //- Reset state of cloud
      void cloudReset(CollidingCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    CollidingCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const volScalarField& mu,
      const dimensionedVector& g,
      bool readFields = true
    );
    //- Copy constructor with new name
    CollidingCloud
    (
      CollidingCloud<CloudType>& c,
      const word& name
    );
    //- Copy constructor with new name - creates bare cloud
    CollidingCloud
    (
      const fvMesh& mesh,
      const word& name,
      const CollidingCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType> > clone(const word& name)
    {
      return autoPtr<Cloud<parcelType> >
      (
        new CollidingCloud(*this, name)
      );
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType> > cloneBare(const word& name) const
    {
      return autoPtr<Cloud<parcelType> >
      (
        new CollidingCloud(this->mesh(), name, *this)
      );
    }
  //- Destructor
  virtual ~CollidingCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const CollidingCloud& cloudCopy() const;
      //- Return the constant properties
      inline const typename parcelType::constantProperties&
        constProps() const;
      //- If the collision model controls the wall interaction,
      //  then the wall impact distance should be zero.
      //  Otherwise, it should be allowed to be the value from
      //  the Parcel.
      virtual bool hasWallImpactDistance() const;
      // Sub-models
        //- Return const access to the collision model
        inline const CollisionModel<CollidingCloud<CloudType> >&
          collision() const;
        //- Return reference to the collision model
        inline CollisionModel<CollidingCloud<CloudType> >&
          collision();
    // Check
      //- Total rotational kinetic energy in the system
      inline scalar rotationalKineticEnergyOfSystem() const;
    // Cloud evolution functions
      //- Store the current cloud state
      void storeState();
      //- Reset the current cloud to the previously stored state
      void restoreState();
      //- Evolve the cloud
      void evolve();
      //- Particle motion
      template<class TrackData>
      void motion(TrackData& td);
    // I-O
      //- Print cloud information
      void info();
};
}  // namespace mousse
#include "_colliding_cloud_i.hpp"
#ifdef NoRepository
#   include "_colliding_cloud.cpp"
#endif
#endif
