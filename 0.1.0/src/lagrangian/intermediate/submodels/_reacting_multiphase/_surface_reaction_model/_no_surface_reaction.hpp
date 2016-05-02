#ifndef LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TSURFACE_REACTION_MODEL_TNO_SURFACE_REACTION_HPP_
#define LAGRANGIAN_INTERMEDIATE_SUBMODELS_TREACTING_MULTIPHASE_TSURFACE_REACTION_MODEL_TNO_SURFACE_REACTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::NoSurfaceReaction
// Description
//   Dummy surface reaction model for 'none'

#include "_surface_reaction_model.hpp"


namespace mousse {

template<class CloudType>
class NoSurfaceReaction
:
  public SurfaceReactionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from dictionary
    NoSurfaceReaction(const dictionary& dict, CloudType& owner);
    //- Construct copy
    NoSurfaceReaction(const NoSurfaceReaction<CloudType>& srm);
    //- Construct and return a clone
    virtual autoPtr<SurfaceReactionModel<CloudType>> clone() const
    {
      return
        autoPtr<SurfaceReactionModel<CloudType>>
        {
          new NoSurfaceReaction<CloudType>{*this}
        };
    }
  //- Destructor
  virtual ~NoSurfaceReaction();
  // Member Functions
    //- Flag to indicate whether model activates devolatisation model
    virtual bool active() const;
    //- Update surface reactions
    virtual scalar calculate
    (
      const scalar dt,
      const label cellI,
      const scalar d,
      const scalar T,
      const scalar Tc,
      const scalar pc,
      const scalar rhoc,
      const scalar mass,
      const scalarField& YGas,
      const scalarField& YLiquid,
      const scalarField& YSolid,
      const scalarField& YMixture,
      const scalar N,
      scalarField& dMassGas,
      scalarField& dMassLiquid,
      scalarField& dMassSolid,
      scalarField& dMassSRCarrier
    ) const;
};

}  // namespace mousse

#include "_no_surface_reaction.ipp"

#endif
