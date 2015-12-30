// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::CollisionModel
// Description
//   Templated collision model class.
// SourceFiles
//   _collision_model.cpp
//   _collision_model_new.cpp
#ifndef _collision_model_hpp_
#define _collision_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class CollisionModel
:
  public CloudSubModelBase<CloudType>
{
  // Protected data
    //- Convenience typedef for parcel type
    typedef typename CloudType::parcelType parcelType;
public:
  //- Runtime type information
  TypeName("collisionModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    CollisionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    CollisionModel(CloudType& owner);
    //- Construct from components
    CollisionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    CollisionModel(const CollisionModel<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<CollisionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~CollisionModel();
  //- Selector
  static autoPtr<CollisionModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Return the number of times to subcycle the current
    //  timestep to meet the criteria of the collision model
    virtual label nSubCycles() const = 0;
    //- Indicates whether model determines wall collisions or not,
    //  used to determine what value to use for wallImpactDistance
    virtual bool controlsWallInteraction() const = 0;
    // Collision function
    virtual void collide() = 0;
};
}  // namespace mousse

#define makeCollisionModel(CloudType)                                         \
                                                                              \
  typedef mousse::CloudType::collidingCloudType collidingCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::CollisionModel<collidingCloudType>,                               \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      CollisionModel<collidingCloudType>,                                     \
      dictionary                                                              \
    );                                                                        \
  }

#define makeCollisionModelType(SS, CloudType)                                 \
                                                                              \
  typedef mousse::CloudType::collidingCloudType collidingCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<collidingCloudType>, 0);     \
                                                                              \
  mousse::CollisionModel<collidingCloudType>::                                \
    adddictionaryConstructorToTable<mousse::SS<collidingCloudType> >          \
      add##SS##CloudType##collidingCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_collision_model.cpp"
#endif
#endif
