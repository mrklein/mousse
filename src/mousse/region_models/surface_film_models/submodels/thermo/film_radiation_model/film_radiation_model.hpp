#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_FILM_RADIATION_MODEL_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_FILM_RADIATION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmRadiationModel
// Description
//   Base class for film radiation models

#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class filmRadiationModel
:
  public filmSubModelBase
{
public:
  //- Runtime type information
  TYPE_NAME("radiationModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      filmRadiationModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    filmRadiationModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    filmRadiationModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    filmRadiationModel(const filmRadiationModel&) = delete;
    //- Disallow default bitwise assignment
    filmRadiationModel& operator=(const filmRadiationModel&) = delete;
  // Selectors
    //- Return a reference to the selected phase change model
    static autoPtr<filmRadiationModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~filmRadiationModel();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct() = 0;
      //- Return the radiation sensible enthalpy source
      virtual tmp<volScalarField> Shs() = 0;
};

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#endif

