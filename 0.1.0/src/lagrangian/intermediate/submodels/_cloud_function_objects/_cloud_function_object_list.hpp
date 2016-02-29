#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TCLOUD_FUNCTION_OBJECT_LIST_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_FUNCTION_OBJECTS_TCLOUD_FUNCTION_OBJECT_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CloudFunctionObjectList
// Description
//   List of cloud function objects
// SourceFiles
//   _cloud_function_object_list.cpp
#include "ptr_list.hpp"
#include "_cloud_function_object.hpp"
namespace mousse
{
template<class CloudType>
class CloudFunctionObjectList
:
  public PtrList<CloudFunctionObject<CloudType> >
{
protected:
  // Protected Data
    //- Reference to the owner cloud
    const CloudType& owner_;
    //- Dictionary
    const dictionary dict_;
public:
  // Constructors
    //- Null constructor
    CloudFunctionObjectList(CloudType& owner);
    //- Construct from mesh
    CloudFunctionObjectList
    (
      CloudType& owner,
      const dictionary& dict,
      const bool readFields
    );
    //- Construct copy
    CloudFunctionObjectList(const CloudFunctionObjectList& ppml);
  //- Destructor
  virtual ~CloudFunctionObjectList();
  // Member Functions
    // Access
      //- Return const access to the cloud owner
      inline const CloudType& owner() const;
      //- Return refernce to the cloud owner
      inline CloudType& owner();
      //- Return the forces dictionary
      inline const dictionary& dict() const;
    // Evaluation
      //- Pre-evolve hook
      virtual void preEvolve();
      //- Post-evolve hook
      virtual void postEvolve();
      //- Post-move hook
      virtual void postMove
      (
        typename CloudType::parcelType& p,
        const label cellI,
        const scalar dt,
        const point& position0,
        bool& keepParticle
      );
      //- Post-patch hook
      virtual void postPatch
      (
        const typename CloudType::parcelType& p,
        const polyPatch& pp,
        const scalar trackFraction,
        const tetIndices& tetIs,
        bool& keepParticle
      );
      //- Post-face hook
      virtual void postFace
      (
        const typename CloudType::parcelType& p,
        const label faceI,
        bool& keepParticle
      );
};
}  // namespace mousse

template<class CloudType>
inline const CloudType& mousse::CloudFunctionObjectList<CloudType>::owner() const
{
  return owner_;
}
template<class CloudType>
inline CloudType& mousse::CloudFunctionObjectList<CloudType>::owner()
{
  return owner_;
}
template<class CloudType>
inline const mousse::dictionary&
mousse::CloudFunctionObjectList<CloudType>::dict() const
{
  return dict_;
}
#ifdef NoRepository
  #include "_cloud_function_object_list.cpp"
#endif
#endif
