#ifndef LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TMAXWELLIAN_THERMAL_HPP_
#define LAGRANGIAN_DSMC_SUBMODELS_TWALL_INTERACTION_MODEL_TMAXWELLIAN_THERMAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::MaxwellianThermal
// Description
//   Wall interaction setting microscopic velocity to a random one
//   drawn from a Maxwellian distribution corresponding to a specified
//   temperature

#include "_wall_interaction_model.hpp"


namespace mousse {

template<class CloudType>
class MaxwellianThermal
:
  public WallInteractionModel<CloudType>
{
public:
  //- Runtime type information
  TYPE_NAME("MaxwellianThermal");
  // Constructors
    //- Construct from dictionary
    MaxwellianThermal
    (
      const dictionary& dict,
      CloudType& cloud
    );
  //- Destructor
  virtual ~MaxwellianThermal();
  // Member Functions
    //- Apply wall correction
    virtual void correct
    (
      typename CloudType::parcelType& p,
      const wallPolyPatch& wpp
    );
};

}  // namespace mousse

#include "_maxwellian_thermal.ipp"

#endif
