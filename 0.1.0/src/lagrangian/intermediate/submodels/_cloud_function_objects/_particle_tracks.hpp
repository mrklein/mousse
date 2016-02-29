#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_TRACKS_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TPARTICLE_TRACKS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ParticleTracks
// Description
//   Records particle state (all variables) on each call to postFace
// SourceFiles
//   _particle_tracks.cpp
#include "_cloud_function_object.hpp"
namespace mousse
{
template<class CloudType>
class ParticleTracks
:
  public CloudFunctionObject<CloudType>
{
  // Private data
    // Typedefs
      //- Convenience typedef for parcel type
      typedef typename CloudType::parcelType parcelType;
      //- Convenience typedef for hash hit-table
      typedef HashTable<label, labelPair, typename labelPair::Hash<> >
        hitTableType;
    //- Number of face-hit intervals between storing parcel data
    label trackInterval_;
    //- Maximum number of particles to store per track
    label maxSamples_;
    //- Flag to indicate whether data should be reset/cleared on writing
    Switch resetOnWrite_;
    //- Table of number of times a particle has hit a face
    hitTableType faceHitCounter_;
    //- Pointer to the cloud storage
    autoPtr<Cloud<parcelType> > cloudPtr_;
protected:
  // Protected member functions
    //- Write post-processing info
    void write();
public:
  //- Runtime type information
  TYPE_NAME("particleTracks");
  // Constructors
    //- Construct from dictionary
    ParticleTracks
    (
      const dictionary& dict,
      CloudType& owner,
      const word& modelName
    );
    //- Construct copy
    ParticleTracks(const ParticleTracks<CloudType>& ppm);
    //- Construct and return a clone
    virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
    {
      return autoPtr<CloudFunctionObject<CloudType> >
      (
        new ParticleTracks<CloudType>(*this)
      );
    }
  //- Destructor
  virtual ~ParticleTracks();
  // Member Functions
    // Access
      //- Return const access to the track interval
      inline label trackInterval() const;
      //- Return const access to the max samples
      inline label maxSamples() const;
      //- Return const access to the reset on write flag
      inline const Switch& resetOnWrite() const;
      //- Rerurn the table of number of times a particle has hit a face
      inline const hitTableType& faceHitCounter() const;
      //- Return const access to the cloud
      inline const Cloud<parcelType>& cloud() const;
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-face hook
      virtual void postFace
      (
        const parcelType& p,
        const label faceI,
        bool& keepParticle
      );
};
}  // namespace mousse

template<class CloudType>
inline mousse::label mousse::ParticleTracks<CloudType>::trackInterval() const
{
  return trackInterval_;
}
template<class CloudType>
inline mousse::label mousse::ParticleTracks<CloudType>::maxSamples() const
{
  return maxSamples_;
}
template<class CloudType>
inline const mousse::Switch& mousse::ParticleTracks<CloudType>::resetOnWrite() const
{
  return resetOnWrite_;
}
template<class CloudType>
inline const typename mousse::ParticleTracks<CloudType>::hitTableType&
mousse::ParticleTracks<CloudType>::faceHitCounter() const
{
  return faceHitCounter_;
}
template<class CloudType>
inline const mousse::Cloud<typename CloudType::parcelType>&
mousse::ParticleTracks<CloudType>::cloud() const
{
  return cloudPtr_();
}
#ifdef NoRepository
  #include "_particle_tracks.cpp"
#endif
#endif
