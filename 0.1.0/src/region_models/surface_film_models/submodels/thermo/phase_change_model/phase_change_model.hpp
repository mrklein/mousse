// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::phaseChangeModel
// Description
//   Base class for surface film phase change models
// SourceFiles
//   phase_change_model.cpp
//   phase_change_model_new.cpp
#ifndef phase_change_model_hpp_
#define phase_change_model_hpp_
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
#include "scalar_field.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class phaseChangeModel
:
  public filmSubModelBase
{
protected:
  // Protected Member Functions
    //- Latest mass transfer due to phase change
    scalar latestMassPC_;
    //- Total mass transfer due to phase change
    scalar totalMassPC_;
public:
  //- Runtime type information
  TYPE_NAME("phaseChangeModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      phaseChangeModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    phaseChangeModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    phaseChangeModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    phaseChangeModel(const phaseChangeModel&) = delete;
    //- Disallow default bitwise assignment
    phaseChangeModel& operator=(const phaseChangeModel&) = delete;
  // Selectors
    //- Return a reference to the selected phase change model
    static autoPtr<phaseChangeModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~phaseChangeModel();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        const scalar dt,
        scalarField& availableMass,
        volScalarField& dMass,
        volScalarField& dEnergy
      );
      //- Correct
      virtual void correctModel
      (
        const scalar dt,
        scalarField& availableMass,
        scalarField& dMass,
        scalarField& dEnergy
      ) = 0;
    // I-O
      //- Provide some feedback
      virtual void info(Ostream& os) const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
