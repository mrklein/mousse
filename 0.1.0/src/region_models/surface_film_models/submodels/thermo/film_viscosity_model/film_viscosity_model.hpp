#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_FILM_VISCOSITY_MODEL_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_VISCOSITY_MODEL_FILM_VISCOSITY_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmViscosityModel
// Description
//   Base class for surface film viscosity models
// SourceFiles
//   film_viscosity_model.cpp
//   film_viscosity_model_new.cpp
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
#include "scalar_field.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class filmViscosityModel
:
  public filmSubModelBase
{
protected:
  // Protected Member Data
    //- Reference to the viscosity field
    volScalarField& mu_;
public:
  //- Runtime type information
  TYPE_NAME("filmViscosityModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      filmViscosityModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict,
        volScalarField& mu
      ),
      (owner, dict, mu)
    );
  // Constructors
    //- Construct from type name, dictionary and surface film model
    filmViscosityModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
    //- Disallow default bitwise copy construct
    filmViscosityModel(const filmViscosityModel&) = delete;
    //- Disallow default bitwise assignment
    filmViscosityModel& operator=(const filmViscosityModel&) = delete;
  // Selectors
    //- Return a reference to the selected phase change model
    static autoPtr<filmViscosityModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
  //- Destructor
  virtual ~filmViscosityModel();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        const volScalarField& p,
        const volScalarField& T
      ) = 0;
    // I-O
      //- Provide some feedback
      virtual void info(Ostream& os) const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
