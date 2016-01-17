// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ReactingMultiphaseCloud
// Description
//   Templated base class for multiphase reacting cloud
//   - Adds to reacting cloud
//    - multiphase composition
//    - devolatilisatsion
//    - surface reactions
// SourceFiles
//   _reacting_multiphase_cloud_i.hpp
//   _reacting_multiphase_cloud.cpp
#ifndef _reacting_multiphase_cloud_hpp_
#define _reacting_multiphase_cloud_hpp_
#include "reacting_multiphase_cloud.hpp"
namespace mousse
{
// Forward declaration of classes
template<class CloudType>
class DevolatilisationModel;
template<class CloudType>
class SurfaceReactionModel;
template<class CloudType>
class ReactingMultiphaseCloud
:
  public CloudType,
  public reactingMultiphaseCloud
{
public:
  // Public typedefs
    //- Type of cloud this cloud was instantiated for
    typedef CloudType cloudType;
    //- Type of parcel the cloud was instantiated for
    typedef typename CloudType::particleType parcelType;
    //- Convenience typedef for this cloud type
    typedef ReactingMultiphaseCloud<CloudType> reactingMultiphaseCloudType;
private:
  // Private data
    //- Cloud copy pointer
    autoPtr<ReactingMultiphaseCloud<CloudType>> cloudCopyPtr_;
protected:
  // Protected data
    //- Parcel constant properties
    typename parcelType::constantProperties constProps_;
    // References to the cloud sub-models
      //- Devolatilisation model
      autoPtr
      <
        DevolatilisationModel<ReactingMultiphaseCloud<CloudType>>
      >
      devolatilisationModel_;
      //- Surface reaction model
      autoPtr
      <
        SurfaceReactionModel<ReactingMultiphaseCloud<CloudType>>
      >
      surfaceReactionModel_;
    // Check
      //- Total mass transferred to continuous phase via devolatilisation
      scalar dMassDevolatilisation_;
      //- Total mass transferred to continuous phase via surface
      //  reactions
      scalar dMassSurfaceReaction_;
  // Protected Member Functions
    // Initialisation
      //- Set cloud sub-models
      void setModels();
    // Cloud evolution functions
      //- Reset state of cloud
      void cloudReset(ReactingMultiphaseCloud<CloudType>& c);
public:
  // Constructors
    //- Construct given carrier gas fields
    ReactingMultiphaseCloud
    (
      const word& cloudName,
      const volScalarField& rho,
      const volVectorField& U,
      const dimensionedVector& g,
      const SLGThermo& thermo,
      bool readFields = true
    );
    //- Copy constructor with new name
    ReactingMultiphaseCloud
    (
      ReactingMultiphaseCloud<CloudType>& c,
      const word& name
    );
    //- Copy constructor with new name - creates bare cloud
    ReactingMultiphaseCloud
    (
      const fvMesh& mesh,
      const word& name,
      const ReactingMultiphaseCloud<CloudType>& c
    );
    //- Construct and return clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> clone(const word& name)
    {
      return autoPtr<Cloud<parcelType>>
      (
        new ReactingMultiphaseCloud(*this, name)
      );
    }
    //- Construct and return bare clone based on (this) with new name
    virtual autoPtr<Cloud<parcelType>> cloneBare(const word& name) const
    {
      return autoPtr<Cloud<parcelType>>
      (
        new ReactingMultiphaseCloud(this->mesh(), name, *this)
      );
    }
    //- Disallow default bitwise copy construct
    ReactingMultiphaseCloud(const ReactingMultiphaseCloud&) = delete;
    //- Disallow default bitwise assignment
    ReactingMultiphaseCloud& operator=
    (
      const ReactingMultiphaseCloud&
    ) = delete;
  //- Destructor
  virtual ~ReactingMultiphaseCloud();
  // Member Functions
    // Access
      //- Return a reference to the cloud copy
      inline const ReactingMultiphaseCloud& cloudCopy() const;
      //- Return the constant properties
      inline const typename parcelType::constantProperties&
        constProps() const;
      //- Return access to the constant properties
      inline typename parcelType::constantProperties& constProps();
      // Sub-models
        //- Return const access to devolatilisation model
        inline const DevolatilisationModel
        <
          ReactingMultiphaseCloud<CloudType>
        >&
        devolatilisation() const;
        //- Return reference to devolatilisation model
        inline DevolatilisationModel
        <
          ReactingMultiphaseCloud<CloudType>
        >&
        devolatilisation();
        //- Return const access to reacting surface reaction model
        inline const SurfaceReactionModel
        <
          ReactingMultiphaseCloud<CloudType>
        >&
        surfaceReaction() const;
        //- Return reference to reacting surface reaction model
        inline SurfaceReactionModel
        <
          ReactingMultiphaseCloud<CloudType>
        >&
        surfaceReaction();
    // Cloud evolution functions
      //- Set parcel thermo properties
      void setParcelThermoProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt
      );
      //- Check parcel properties
      void checkParcelProperties
      (
        parcelType& parcel,
        const scalar lagrangianDt,
        const bool fullyDescribed
      );
      //- Store the current cloud state
      void storeState();
      //- Reset the current cloud to the previously stored state
      void restoreState();
      //- Reset the cloud source terms
      void resetSourceTerms();
      //- Evolve the cloud
      void evolve();
    // Mapping
      //- Remap the cells of particles corresponding to the
      //  mesh topology change with a default tracking data object
      virtual void autoMap(const mapPolyMesh&);
    // I-O
      //- Print cloud information
      void info();
      //- Write the field data for the cloud
      virtual void writeFields() const;
};
}  // namespace mousse

// Member Functions 
template<class CloudType>
inline const mousse::ReactingMultiphaseCloud<CloudType>&
mousse::ReactingMultiphaseCloud<CloudType>::cloudCopy() const
{
  return cloudCopyPtr_();
}
template<class CloudType>
inline const typename CloudType::particleType::constantProperties&
mousse::ReactingMultiphaseCloud<CloudType>::constProps() const
{
  return constProps_;
}
template<class CloudType>
inline typename CloudType::particleType::constantProperties&
mousse::ReactingMultiphaseCloud<CloudType>::constProps()
{
  return constProps_;
}
template<class CloudType>
inline const mousse::DevolatilisationModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::devolatilisation() const
{
  return devolatilisationModel_;
}
template<class CloudType>
inline mousse::DevolatilisationModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::devolatilisation()
{
  return devolatilisationModel_();
}
template<class CloudType>
inline const mousse::SurfaceReactionModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::surfaceReaction() const
{
  return surfaceReactionModel_;
}
template<class CloudType>
inline mousse::SurfaceReactionModel
<
  mousse::ReactingMultiphaseCloud<CloudType>
>&
mousse::ReactingMultiphaseCloud<CloudType>::surfaceReaction()
{
  return surfaceReactionModel_();
}

#ifdef NoRepository
  #include "_reacting_multiphase_cloud.cpp"
#endif
#endif
