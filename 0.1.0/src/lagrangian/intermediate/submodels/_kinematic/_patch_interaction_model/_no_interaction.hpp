#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPATCH_INTERACTION_MODEL_TNO_INTERACTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TKINEMATIC_TPATCH_INTERACTION_MODEL_TNO_INTERACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoInteraction
// Description
//   Dummy class for 'none' option - will raise an error if any functions are
//   called that require return values.

#include "_patch_interaction_model.hpp"


namespace mousse {

template<class CloudType>
class NoInteraction
:
  public PatchInteractionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoInteraction(const dictionary& dict, CloudType& cloud);
    //- Construct copy
    NoInteraction(const NoInteraction<CloudType>& pim);
    //- Construct and return a clone
    virtual autoPtr<PatchInteractionModel<CloudType>> clone() const
    {
      return
        autoPtr<PatchInteractionModel<CloudType>>
        {
          new NoInteraction<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~NoInteraction();
  // Member Functions
    //- Flag to indicate whether model activates patch interaction model
    virtual bool active() const;
    //- Apply velocity correction
    //  Returns true if particle remains in same cell
    virtual bool correct
    (
      typename CloudType::parcelType& p,
      const polyPatch& pp,
      bool& keepParticle,
      const scalar trackFraction,
      const tetIndices& tetIs
    );
};

}  // namespace mousse

#include "_no_interaction.ipp"

#endif
