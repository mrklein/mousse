// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_cloud_function_object_list.hpp"
#include "entry.hpp"
// Constructors 
template<class CloudType>
mousse::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
  CloudType& owner
)
:
  PtrList<CloudFunctionObject<CloudType> >(),
  owner_(owner),
  dict_(dictionary::null)
{}
template<class CloudType>
mousse::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
  CloudType& owner,
  const dictionary& dict,
  const bool readFields
)
:
  PtrList<CloudFunctionObject<CloudType> >(),
  owner_(owner),
  dict_(dict)
{
  if (readFields)
  {
    wordList modelNames(dict.toc());
    Info<< "Constructing cloud functions" << endl;
    if (modelNames.size() > 0)
    {
      this->setSize(modelNames.size());
      FOR_ALL(modelNames, i)
      {
        const word& modelName = modelNames[i];
        const dictionary& modelDict(dict.subDict(modelName));
        // read the type of the function object
        const word objectType(modelDict.lookup("type"));
        this->set
        (
          i,
          CloudFunctionObject<CloudType>::New
          (
            modelDict,
            owner,
            objectType,
            modelName
          )
        );
      }
    }
    else
    {
      Info<< "    none" << endl;
    }
  }
}
template<class CloudType>
mousse::CloudFunctionObjectList<CloudType>::CloudFunctionObjectList
(
  const CloudFunctionObjectList& cfol
)
:
  PtrList<CloudFunctionObject<CloudType> >(cfol),
  owner_(cfol.owner_),
  dict_(cfol.dict_)
{}
// Destructor 
template<class CloudType>
mousse::CloudFunctionObjectList<CloudType>::~CloudFunctionObjectList()
{}
// Member Functions 
template<class CloudType>
void mousse::CloudFunctionObjectList<CloudType>::preEvolve()
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).preEvolve();
  }
}
template<class CloudType>
void mousse::CloudFunctionObjectList<CloudType>::postEvolve()
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).postEvolve();
  }
}
template<class CloudType>
void mousse::CloudFunctionObjectList<CloudType>::postMove
(
  typename CloudType::parcelType& p,
  const label cellI,
  const scalar dt,
  const point& position0,
  bool& keepParticle
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).postMove(p, cellI, dt, position0, keepParticle);
    if (!keepParticle)
    {
      return;
    }
  }
}
template<class CloudType>
void mousse::CloudFunctionObjectList<CloudType>::postPatch
(
  const typename CloudType::parcelType& p,
  const polyPatch& pp,
  const scalar trackFraction,
  const tetIndices& tetIs,
  bool& keepParticle
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).postPatch
    (
      p,
      pp,
      trackFraction,
      tetIs,
      keepParticle
    );
    if (!keepParticle)
    {
      return;
    }
  }
}
template<class CloudType>
void mousse::CloudFunctionObjectList<CloudType>::postFace
(
  const typename CloudType::parcelType& p,
  const label faceI,
  bool& keepParticle
)
{
  FOR_ALL(*this, i)
  {
    this->operator[](i).postFace(p, faceI, keepParticle);
    if (!keepParticle)
    {
      return;
    }
  }
}
