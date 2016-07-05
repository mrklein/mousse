#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TCOLLISION_MODEL_TPAIR_COLLISION_TWALL_MODEL_TWALL_MODEL_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TCOLLISION_MODEL_TPAIR_COLLISION_TWALL_MODEL_TWALL_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::WallModel
// Description
//   Templated wall interaction class

#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "run_time_selection_tables.hpp"
#include "_wall_site_data.hpp"


namespace mousse {

template<class CloudType>
class WallModel
{
  // Private data
    //- The CollisionModel dictionary
    const dictionary& dict_;
    //- Reference to the owner cloud class
    CloudType& owner_;
    //- The coefficients dictionary
    const dictionary coeffDict_;
public:
  //- Runtime type information
  TYPE_NAME("wallModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    WallModel,
    dictionary,
    (
      const dictionary& dict,
      CloudType& owner
    ),
    (dict, owner)
  );
  // Constructors
    //- Construct from components
    WallModel
    (
      const dictionary& dict,
      CloudType& owner,
      const word& type
    );
  //- Destructor
  virtual ~WallModel();
  //- Selector
  static autoPtr<WallModel<CloudType>> New
  (
    const dictionary& dict,
    CloudType& owner
  );
  // Access
    //- Return the owner cloud object
    const CloudType& owner() const;
    //- Return non-const access to the owner cloud object
    CloudType& owner();
    //- Return the dictionary
    const dictionary& dict() const;
    //- Return the coefficients dictionary
    const dictionary& coeffDict() const;
  // Member Functions
    //- Return the effective radius for a particle for the model
    virtual scalar pREff(const typename CloudType::parcelType& p) const = 0;
    //- Whether the WallModel has a timestep limit that will
    //  require subCycling
    virtual bool controlsTimestep() const = 0;
    //- For WallModels that control the timestep, calculate the
    //  number of subCycles needed to satisfy the minimum
    //  allowable timestep
    virtual label nSubCycles() const = 0;
    //- Calculate the wall interaction for a parcel
    virtual void evaluateWall
    (
      typename CloudType::parcelType& p,
      const List<point>& flatSitePoints,
      const List<WallSiteData<vector>>& flatSiteData,
      const List<point>& sharpSitePoints,
      const List<WallSiteData<vector>>& sharpSiteData
    ) const = 0;
};

}  // namespace mousse


#define MAKE_WALL_MODEL(CloudType)                                            \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG                                   \
  (                                                                           \
    mousse::WallModel<mousse::CloudType>, 0                                   \
  );                                                                          \
                                                                              \
  namespace mousse {                                                          \
  DEFINE_TEMPLATE_RUN_TIME_SELECTION_TABLE                                    \
  (                                                                           \
    WallModel<mousse::CloudType>,                                             \
    dictionary                                                                \
  );                                                                          \
  }


#define MAKE_WALL_MODEL_TYPE(SS, CloudType)                                   \
                                                                              \
  DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(mousse::SS<mousse::CloudType>, 0);\
                                                                              \
  mousse::WallModel<mousse::CloudType>::                                      \
    adddictionaryConstructorToTable<mousse::SS<mousse::CloudType>>            \
      add##SS##CloudType##ConstructorToTable_;


#include "_wall_model.ipp"

#endif
