#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_PHASE_CHANGE_MODEL_NO_PHASE_CHANGE_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_PHASE_CHANGE_MODEL_NO_PHASE_CHANGE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noPhaseChange
// Description
//   Dummy phase change model for 'none'

#include "phase_change_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class noPhaseChange
:
  public phaseChangeModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from surface film model
    noPhaseChange(surfaceFilmModel& owner, const dictionary& dict);
    //- Disallow default bitwise copy construct
    noPhaseChange(const noPhaseChange&) = delete;
    //- Disallow default bitwise assignment
    noPhaseChange& operator=(const noPhaseChange&) = delete;
  //- Destructor
  virtual ~noPhaseChange();
  // Member Functions
    // Evolution
      //- Return the model 'active' status - default active = true
      virtual bool active() const;
      //- Correct
      virtual void correctModel
      (
        const scalar dt,
        scalarField& availableMass,
        scalarField& dMass,
        scalarField& dEnergy
      );
};

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#endif

