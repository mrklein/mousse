#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_LIQUID_VISCOSITY_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_LIQUID_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liquidViscosity
// Description
//   liquidViscosity viscosity model

#include "film_viscosity_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class liquidViscosity
:
  public filmViscosityModel
{
public:
  //- Runtime type information
  TYPE_NAME("liquid");
  // Constructors
    //- Construct from surface film model
    liquidViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
    //- Disallow default bitwise copy construct
    liquidViscosity(const liquidViscosity&) = delete;
    //- Disallow default bitwise assignment
    liquidViscosity& operator=(const liquidViscosity&) = delete;
  //- Destructor
  virtual ~liquidViscosity();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        const volScalarField& p,
        const volScalarField& T
      );
};

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#endif

