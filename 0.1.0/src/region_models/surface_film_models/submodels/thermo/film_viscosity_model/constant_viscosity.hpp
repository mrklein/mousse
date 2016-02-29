#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_CONSTANT_VISCOSITY_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_CONSTANT_VISCOSITY_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantViscosity
// Description
//   Constant viscosity model
// SourceFiles
//   constant_viscosity.cpp
#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class constantViscosity
:
  public filmViscosityModel
{
protected:
  // Protected data
    //- Constant viscosity [Pa.s]
    dimensionedScalar mu0_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from surface film model
    constantViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
    //- Disallow default bitwise copy construct
    constantViscosity(const constantViscosity&) = delete;
    //- Disallow default bitwise assignment
    constantViscosity& operator=(const constantViscosity&) = delete;
  //- Destructor
  virtual ~constantViscosity();
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
