#ifndef LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TMIXED_DIFFUSE_SPECULAR_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TMIXED_DIFFUSE_SPECULAR_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MixedDiffuseSpecular
// Description
//   Wall interaction setting microscopic velocity to a random one
//   drawn from a Maxwellian distribution corresponding to a specified
//   temperature for a specified fraction of collisions, and reversing
//   the wall-normal component of the particle velocity for the
//   remainder.

#include "_wall_interaction_model.hpp"


namespace mousse {

template<class CloudType>
class MixedDiffuseSpecular
:
  public WallInteractionModel<CloudType>
{
  // Private data
    //- Fraction of wall interactions that are diffuse
    scalar diffuseFraction_;
public:
  //- Runtime type information
  TYPE_NAME("MixedDiffuseSpecular");
  // Constructors
    //- Construct from dictionary
    MixedDiffuseSpecular
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~MixedDiffuseSpecular();
  // Member Functions
    //- Apply wall correction
    virtual void correct
    (
      typename CloudType::parcelType& p,
      const wallPolyPatch& wpp
    );
};

}  // namespace mousse

#include "_mixed_diffuse_specular.ipp"

#endif
