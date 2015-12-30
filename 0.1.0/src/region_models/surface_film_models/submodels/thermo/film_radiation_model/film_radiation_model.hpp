// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::filmRadiationModel
// Description
//   Base class for film radiation models
// SourceFiles
//   film_radiation_model.cpp
//   film_radiation_model_new.cpp
#ifndef film_radiation_model_hpp_
#define film_radiation_model_hpp_
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class filmRadiationModel
:
  public filmSubModelBase
{
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    filmRadiationModel(const filmRadiationModel&);
    //- Disallow default bitwise assignment
    void operator=(const filmRadiationModel&);
public:
  //- Runtime type information
  TypeName("radiationModel");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
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
