#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_HEAT_TRANSFER_MODEL_HEAT_TRANSFER_MODEL_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_HEAT_TRANSFER_MODEL_HEAT_TRANSFER_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::heatTransferModel
// Description
//   Base class for film heat transfer models
// SourceFiles
//   heat_transfer_model.cpp
//   heat_transfer_model_new.cpp
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class heatTransferModel
:
  public filmSubModelBase
{
public:
  //- Runtime type information
  TYPE_NAME("heatTransferModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      heatTransferModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    heatTransferModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    heatTransferModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    heatTransferModel(const heatTransferModel&) = delete;
    //- Disallow default bitwise assignment
    heatTransferModel& operator=(const heatTransferModel&) = delete;
  // Selectors
    //- Return a reference to the selected phase change model
    static autoPtr<heatTransferModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~heatTransferModel();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct() = 0;
      //- Return the heat transfer coefficient [W/m2/K]
      virtual tmp<volScalarField> h() const = 0;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
