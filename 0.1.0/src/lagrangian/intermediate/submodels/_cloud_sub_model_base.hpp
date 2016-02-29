#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_SUB_MODEL_BASE_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TCLOUD_SUB_MODEL_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CloudSubModelBase
// Description
//   Base class for cloud sub-models
// SourceFiles
//   _cloud_sub_model_base.cpp
#include "sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class CloudSubModelBase
:
  public subModelBase
{
protected:
  // Protected Data
    //- Reference to the cloud
    CloudType& owner_;
public:
  // Constructors
    //- Construct null from owner cloud
    CloudSubModelBase(CloudType& owner);
    //- Construct from owner cloud without name
    CloudSubModelBase
    (
      CloudType& owner,
      const dictionary& dict,
      const word& baseName,
      const word& modelType,
      const word& dictExt = "Coeffs"
    );
    //- Construct from owner cloud with name
    CloudSubModelBase
    (
      const word& modelName,
      CloudType& owner,
      const dictionary& dict,
      const word& baseName,
      const word& modelType
    );
    //- Construct as copy
    CloudSubModelBase(const CloudSubModelBase<CloudType>& smb);
  //- Destructor
  virtual ~CloudSubModelBase();
  //- Type of cloud this model was instantiated for
  typedef CloudType cloudType;
  // Member Functions
    // Access
      //- Return const access to the owner cloud
      const CloudType& owner() const;
      //- Flag to indicate when to write a property
      virtual bool outputTime() const;
    // Edit
      //- Return non-const access to the owner cloud for manipulation
      CloudType& owner();
    // I-O
      //- Write
      virtual void write(Ostream& os) const;
};
}  // namespace mousse
#ifdef NoRepository
#   include "_cloud_sub_model_base.cpp"
#endif
#endif
