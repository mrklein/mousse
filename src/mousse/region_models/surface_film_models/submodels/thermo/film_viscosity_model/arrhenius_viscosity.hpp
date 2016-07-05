#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_ARRHENIUS_VISCOSITY_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_ARRHENIUS_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ArrheniusViscosity
// Description
//   The Arrhenius temperature-dependent viscosity model multiplies the viscosity
//   of a base model by an Arrhenius-type temperature function:
//     mu = mu0*exp(k1*(1/(T + k2) - 1/(Tref + k2)))
//   Where:
//     mu0 is the base-model viscosity
//     k1 and k2 are Arrhenius coefficients
//     T is the local temperature
//     Tref is the reference temperature

#include "film_viscosity_model.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class ArrheniusViscosity
:
  public filmViscosityModel
{
protected:
  // Protected data
    //- Base viscosity model
    autoPtr<filmViscosityModel> viscosity_;
    //- Coefficient k1
    dimensionedScalar k1_;
    //- Coefficient k2
    dimensionedScalar k2_;
    //- Reference temperature
    dimensionedScalar Tref_;
public:
  //- Runtime type information
  TYPE_NAME("Arrhenius");
  // Constructors
    //- Construct from surface film model
    ArrheniusViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
    //- Disallow default bitwise copy construct
    ArrheniusViscosity(const ArrheniusViscosity&) = delete;
    //- Disallow default bitwise assignment
    ArrheniusViscosity& operator=(const ArrheniusViscosity&) = delete;
  //- Destructor
  virtual ~ArrheniusViscosity();
  // Member Functions
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

