// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud_function_object.hpp"
// Protected Member Functions 
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::write()
{
  NOT_IMPLEMENTED("void mousse::CloudFunctionObject<CloudType>::write()");
}
// Constructors 
template<class CloudType>
mousse::CloudFunctionObject<CloudType>::CloudFunctionObject(CloudType& owner)
:
  CloudSubModelBase<CloudType>(owner),
  outputDir_()
{}
template<class CloudType>
mousse::CloudFunctionObject<CloudType>::CloudFunctionObject
(
  const dictionary& dict,
  CloudType& owner,
  const word& modelName,
  const word& objectType
)
:
  CloudSubModelBase<CloudType>(modelName, owner, dict, typeName, objectType),
  outputDir_(owner.mesh().time().path())
{
  const fileName relPath =
    "postProcessing"/cloud::prefix/owner.name()/this->modelName();
  if (Pstream::parRun())
  {
    // Put in undecomposed case (Note: gives problems for
    // distributed data running)
    outputDir_ = outputDir_/".."/relPath;
  }
  else
  {
    outputDir_ = outputDir_/relPath;
  }
}
template<class CloudType>
mousse::CloudFunctionObject<CloudType>::CloudFunctionObject
(
  const CloudFunctionObject<CloudType>& ppm
)
:
  CloudSubModelBase<CloudType>(ppm),
  outputDir_(ppm.outputDir_)
{}
// Destructor 
template<class CloudType>
mousse::CloudFunctionObject<CloudType>::~CloudFunctionObject()
{}
// Member Functions 
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::preEvolve()
{
  // do nothing
}
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::postEvolve()
{
  if (this->owner().time().outputTime())
  {
    this->write();
  }
}
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::postMove
(
  typename CloudType::parcelType&,
  const label,
  const scalar,
  const point&,
  bool&
)
{
  // do nothing
}
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::postPatch
(
  const typename CloudType::parcelType&,
  const polyPatch&,
  const scalar,
  const tetIndices&,
  bool&
)
{
  // do nothing
}
template<class CloudType>
void mousse::CloudFunctionObject<CloudType>::postFace
(
  const typename CloudType::parcelType&,
  const label,
  bool&
)
{
  // do nothing
}
template<class CloudType>
const mousse::fileName& mousse::CloudFunctionObject<CloudType>::outputDir() const
{
  return outputDir_;
}
template<class CloudType>
mousse::fileName mousse::CloudFunctionObject<CloudType>::outputTimeDir() const
{
  return outputDir_/this->owner().time().timeName();
}
#include "_cloud_function_object_new.cpp"
