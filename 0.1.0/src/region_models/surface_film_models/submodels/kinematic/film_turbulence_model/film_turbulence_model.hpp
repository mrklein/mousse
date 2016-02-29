#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FILM_TURBULENCE_MODEL_FILM_TURBULENCE_MODEL_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FILM_TURBULENCE_MODEL_FILM_TURBULENCE_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::surfaceFilmModels::filmTurbulenceModel
// Description
//   Base class for film turbulence models
// SourceFiles
//   film_turbulence_model.cpp
//   film_turbulence_model_new.cpp
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_matrices_fwd.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class filmTurbulenceModel
:
  public filmSubModelBase
{
public:
  //- Runtime type information
  TYPE_NAME("filmTurbulenceModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      filmTurbulenceModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    filmTurbulenceModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    filmTurbulenceModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    filmTurbulenceModel(const filmTurbulenceModel&) = delete;
    //- Disallow default bitwise assignment
    filmTurbulenceModel& operator=(const filmTurbulenceModel&) = delete;
  // Selectors
    //- Return a reference to the selected injection model
    static autoPtr<filmTurbulenceModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~filmTurbulenceModel();
  // Member Functions
    // Evolution
      //- Return the film surface velocity
      virtual tmp<volVectorField> Us() const = 0;
      //- Return the film turbulence viscosity
      virtual tmp<volScalarField> mut() const = 0;
      //- Correct/update the model
      virtual void correct() = 0;
      //- Return the source for the film momentum equation
      virtual tmp<fvVectorMatrix> Su(volVectorField& U) const = 0;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
