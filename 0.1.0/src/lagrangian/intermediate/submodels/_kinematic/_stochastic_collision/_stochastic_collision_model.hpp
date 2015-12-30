// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::StochasticCollisionModel
// Description
//   Templated stochastic collision model class
// SourceFiles
//   _stochastic_collision_model.cpp
//   _stochastic_collision_model_new.cpp
#ifndef _stochastic_collision_model_hpp_
#define _stochastic_collision_model_hpp_
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_cloud_sub_model_base.hpp"
namespace mousse
{
template<class CloudType>
class StochasticCollisionModel
:
  public CloudSubModelBase<CloudType>
{
protected:
  //- Main collision routine
  virtual void collide(const scalar dt) = 0;
public:
  //- Runtime type information
  TypeName("collisionModel");
  //- Declare runtime constructor selection table
  declareRunTimeSelectionTable
  (
    autoPtr,
    StochasticCollisionModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct null from owner
    StochasticCollisionModel(CloudType& owner);
    //- Construct from dictionary
    StochasticCollisionModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
    //- Construct copy
    StochasticCollisionModel(const StochasticCollisionModel<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<StochasticCollisionModel<CloudType> > clone() const = 0;
  //- Destructor
  virtual ~StochasticCollisionModel();
  //- Selector
  static autoPtr<StochasticCollisionModel<CloudType> > New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Member Functions
    //- Update the model
    void update(const scalar dt);
};
}  // namespace mousse

#define makeStochasticCollisionModel(CloudType)                               \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug                                         \
  (                                                                           \
    mousse::StochasticCollisionModel<kinematicCloudType>,                     \
    0                                                                         \
  );                                                                          \
  namespace mousse                                                            \
  {                                                                           \
    defineTemplateRunTimeSelectionTable                                       \
    (                                                                         \
      StochasticCollisionModel<kinematicCloudType>,                           \
      dictionary                                                              \
    );                                                                        \
  }

#define makeStochasticCollisionModelType(SS, CloudType)                       \
                                                                              \
  typedef mousse::CloudType::kinematicCloudType kinematicCloudType;           \
  defineNamedTemplateTypeNameAndDebug(mousse::SS<kinematicCloudType>, 0);     \
                                                                              \
  mousse::StochasticCollisionModel<kinematicCloudType>::                      \
    adddictionaryConstructorToTable<mousse::SS<kinematicCloudType> >          \
      add##SS##CloudType##kinematicCloudType##ConstructorToTable_;

#ifdef NoRepository
#   include "_stochastic_collision_model.cpp"
#endif
#endif
