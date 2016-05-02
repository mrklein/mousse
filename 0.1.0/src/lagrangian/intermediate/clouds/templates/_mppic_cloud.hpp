#ifndef LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TMPPIC_CLOUD_HPP_
#define LAGRANGIAN_INTERMEDIATE_CLOUDS_TEMPLATES_TMPPIC_CLOUD_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MPPICCloud
// Description
//   Adds MPPIC modelling to kinematic clouds

#include "particle.hpp"
#include "_cloud.hpp"
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"


namespace mousse {

// Forward declaration of classes
template<class CloudType> class PackingModel;
template<class CloudType> class DampingModel;
template<class CloudType> class IsotropyModel;


template<class CloudType>
class MPPICCloud
:
  public CloudType
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::parcelType parcelType;
    //- Convenience typedef for this cloud type
    typedef MPPICCloud<CloudType> MPPICCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<MPPICCloud<CloudType>> cloudCopyPtr_;
protected:
  // Protected data
    // References to the cloud sub-models
      //- Packing model
      autoPtr<PackingModel<MPPICCloud<CloudType>>> packingModel_;
      //- Damping model
      autoPtr<DampingModel<MPPICCloud<CloudType>>> dampingModel_;
      //- Exchange model
      autoPtr<IsotropyModel<MPPICCloud<CloudType>>> isotropyModel_;
    // Initialisation
      //- Set cloud sub-models
      void setModels();
public:
  // Constructors
    //- Construct given carrier gas fields
    MPPICCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const volScalarField& mu,
      const dimensionedVector& g,
      bool readFields = true
    );
    //- Copy constructor with new name
    MPPICCloud
    (
      MPPICCloud<CloudType>& c,
      const word& name
    );
    //- Copy constructor with new name - creates bare cloud
    MPPICCloud
    (
      const fvMesh& mesh,
      const word& name,
      const MPPICCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new MPPICCloud{*this, name}
        };
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return
        autoPtr<Cloud<parcelType>>
        {
          new MPPICCloud{this->mesh(), name, *this}
        };
    }
    //- Disallow default bitwise copy construct
    MPPICCloud(const MPPICCloud&) = delete;
    //- Disallow default bitwise assignment
    MPPICCloud& operator=(const MPPICCloud&) = delete;
  //- Destructor
  virtual ~MPPICCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const MPPICCloud& cloudCopy() const;
      //- Return const access to the packing model
      inline const PackingModel<MPPICCloud<CloudType>>&
        packingModel() const;
      //- Return a reference to the packing model
      inline PackingModel<MPPICCloud<CloudType>>& packingModel();
      //- Return condt access to the damping model
      inline const DampingModel<MPPICCloud<CloudType>>&
        dampingModel() const;
      //- Return a reference to the damping model
      inline DampingModel<MPPICCloud<CloudType>>& dampingModel();
      //- Return condt access to the isotropy model
      inline const IsotropyModel<MPPICCloud<CloudType>>&
        isotropyModel() const;
      //- Return a reference to the isotropy model
      inline IsotropyModel<MPPICCloud<CloudType>>& isotropyModel();
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
    //- I-O
      //- Print cloud information
      void info();
};

}  // namespace mousse


// Member Functions 
template<class CloudType>
inline const mousse::MPPICCloud<CloudType>&
mousse::MPPICCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}


template<class CloudType>
inline const mousse::PackingModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::packingModel() const
{
  return packingModel_();
}


template<class CloudType>
inline mousse::PackingModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::packingModel()
{
  return packingModel_();
}


template<class CloudType>
inline const mousse::DampingModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::dampingModel() const
{
  return dampingModel_();
}


template<class CloudType>
inline mousse::DampingModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::dampingModel()
{
  return dampingModel_();
}


template<class CloudType>
inline const mousse::IsotropyModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::isotropyModel() const
{
  return isotropyModel_();
}


template<class CloudType>
inline mousse::IsotropyModel<mousse::MPPICCloud<CloudType>>&
mousse::MPPICCloud<CloudType>::isotropyModel()
{
  return isotropyModel_();
}

#include "_mppic_cloud.ipp"

#endif
