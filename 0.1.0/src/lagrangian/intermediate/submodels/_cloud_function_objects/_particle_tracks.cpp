// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_particle_tracks.hpp"
#include "pstream.hpp"
#include "list_list_ops.hpp"
#include "ioptr_list.hpp"
// protected Member Functions 
template<class CloudType>
void mousse::ParticleTracks<CloudType>::write()
{
  if (cloudPtr_.valid())
  {
    cloudPtr_->write();
    if (resetOnWrite_)
    {
      cloudPtr_->clear();
    }
  }
  else
  {
    if (debug)
    {
      Info<< "void mousse::ParticleTracks<CloudType>::write()" << nl
        << "cloupPtr invalid" << endl;
    }
  }
}
// Constructors 
template<class CloudType>
mousse::ParticleTracks<CloudType>::ParticleTracks
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName
)
:
  CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
  trackInterval_(readLabel(this->coeffDict().lookup("trackInterval"))),
  maxSamples_(readLabel(this->coeffDict().lookup("maxSamples"))),
  resetOnWrite_(this->coeffDict().lookup("resetOnWrite")),
  faceHitCounter_(),
  cloudPtr_(NULL)
{}
template<class CloudType>
mousse::ParticleTracks<CloudType>::ParticleTracks
(
  const ParticleTracks<CloudType>& ppm
)
:
  CloudFunctionObject<CloudType>(ppm),
  trackInterval_(ppm.trackInterval_),
  maxSamples_(ppm.maxSamples_),
  resetOnWrite_(ppm.resetOnWrite_),
  faceHitCounter_(ppm.faceHitCounter_),
  cloudPtr_(ppm.cloudPtr_)
{}
// Destructor 
template<class CloudType>
mousse::ParticleTracks<CloudType>::~ParticleTracks()
{}
// Member Functions 
template<class CloudType>
void mousse::ParticleTracks<CloudType>::preEvolve()
{
  if (!cloudPtr_.valid())
  {
    cloudPtr_.reset
    (
      this->owner().cloneBare(this->owner().name() + "Tracks").ptr()
    );
  }
}
template<class CloudType>
void mousse::ParticleTracks<CloudType>::postFace
(
  const parcelType& p,
  const label,
  bool&
)
{
  if
  (
    this->owner().solution().output()
  || this->owner().solution().transient()
  )
  {
    if (!cloudPtr_.valid())
    {
      FATAL_ERROR_IN
      (
        "mousse::ParticleTracks<CloudType>::postFace"
        "("
          "const parcelType&, "
          "const label"
        ")"
      )<< "Cloud storage not allocated" << abort(FatalError);
    }
    hitTableType::iterator iter =
      faceHitCounter_.find(labelPair(p.origProc(), p.origId()));
    label localI = -1;
    if (iter != faceHitCounter_.end())
    {
      iter()++;
      localI = iter();
    }
    else
    {
      localI = 1;
      faceHitCounter_.insert(labelPair(p.origProc(), p.origId()), localI);
    }
    label nSamples = floor(localI/trackInterval_);
    if ((localI % trackInterval_ == 0) && (nSamples < maxSamples_))
    {
      cloudPtr_->append
      (
        static_cast<parcelType*>(p.clone(this->owner().mesh()).ptr())
      );
    }
  }
}
