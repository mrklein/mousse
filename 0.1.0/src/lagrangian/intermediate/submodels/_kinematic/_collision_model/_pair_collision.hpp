#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TCOLLISION_MODEL_TPAIR_COLLISION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TCOLLISION_MODEL_TPAIR_COLLISION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PairCollision

#include "_collision_model.hpp"
#include "_interaction_lists.hpp"
#include "_wall_site_data.hpp"


namespace mousse {

// Forward declaration of classes
template<class CloudType> class PairModel;
template<class CloudType> class WallModel;


template<class CloudType>
class PairCollision
:
  public CollisionModel<CloudType>
{
  // Static data
    //- Tolerance to determine flat wall interactions
    static scalar cosPhiMinFlatWall;
    //- Distance to multiply the distance to a wall with within
    //  which a duplicate flat wall interaction is defined.  If
    //  cosPhiFlatWall = 1 - e, this = sqrt(2e - e^2) < sqrt(3e)
    //  for all e < 1, giving a safe tolerance (i.e. no double
    //  interactions next to each other.)
    static scalar flatWallDuplicateExclusion;
  // Private data
    //- PairModel to calculate the interaction between two parcels
    autoPtr<PairModel<CloudType>> pairModel_;
    //- WallModel to calculate the interaction between the parcel and walls
    autoPtr<WallModel<CloudType>> wallModel_;
    //- Interactions lists determining which cells are in
    //  interaction range of each other
    InteractionLists<typename CloudType::parcelType> il_;
  // Private member functions
    //- Pre collision tasks
    void preInteraction();
    //- Interactions between parcels
    void parcelInteraction();
    //- Interactions between real (on-processor) particles
    void realRealInteraction();
    //- Interactions between real and referred (off processor) particles
    void realReferredInteraction();
    //- Interactions with walls
    void wallInteraction();
    bool duplicatePointInList
    (
      const DynamicList<point>& existingPoints,
      const point& pointToTest,
      scalar duplicateRangeSqr
    ) const;
    bool duplicatePointInList
    (
      const DynamicList<point>& existingPoints,
      const point& pointToTest,
      const scalarList& duplicateRangeSqr
    ) const;
    //- Post collision tasks
    void postInteraction();
    //- Calculate the pair force between parcels
    void evaluatePair
    (
      typename CloudType::parcelType& pA,
      typename CloudType::parcelType& pB
    ) const;
    //- Calculate the wall forces on a parcel
    void evaluateWall
    (
      typename CloudType::parcelType& p,
      const List<point>& flatSitePoints,
      const List<WallSiteData<vector>>& flatSiteData,
      const List<point>& sharpSitePoints,
      const List<WallSiteData<vector>>& sharpSiteData
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("pairCollision");
  // Constructors
    //- Construct from components
    PairCollision(const dictionary& dict, CloudType& owner);
    //- Construct copy
    PairCollision(const PairCollision<CloudType>& cm);
    //- Construct and return a clone
    virtual autoPtr<CollisionModel<CloudType>> clone() const
    {
      return
        autoPtr<CollisionModel<CloudType>>
        {
          new PairCollision<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~PairCollision();
  // Member Functions
    //- Return the number of times to subcycle the current
    //  timestep to meet the criteria of the collision model.
    virtual label nSubCycles() const;
    //- Indicates whether model determines wall collisions or not,
    //  used to determine what value to use for wallImpactDistance
    virtual bool controlsWallInteraction() const;
    // Collision function
    virtual void collide();
};

}  // namespace mousse

#include "_pair_collision.ipp"

#endif
