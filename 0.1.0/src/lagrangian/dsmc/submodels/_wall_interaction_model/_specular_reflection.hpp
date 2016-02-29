#ifndef LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TSPECULAR_REFLECTION_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TSPECULAR_REFLECTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SpecularReflection
// Description
//   Reversing the wall-normal component of the particle velocity


#include "_wall_interaction_model.hpp"

namespace mousse
{
template<class CloudType>
class SpecularReflection
:
  public WallInteractionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("SpecularReflection");
  // Constructors
    //- Construct from dictionary
    SpecularReflection
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~SpecularReflection();
  // Member Functions
    //- Apply wall correction
    virtual void correct
    (
      typename CloudType::parcelType& p,
      const wallPolyPatch& wpp
    );
};
}  // namespace mousse

#ifdef NoRepository
#   include "_specular_reflection.cpp"
#endif
#endif
