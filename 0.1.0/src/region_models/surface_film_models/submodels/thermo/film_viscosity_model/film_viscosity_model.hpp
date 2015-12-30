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
#ifndef film_viscosity_model_hpp_
#define film_viscosity_model_hpp_
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
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    filmViscosityModel(const filmViscosityModel&);
    //- Disallow default bitwise assignment
    void operator=(const filmViscosityModel&);
protected:
  // Protected Member Data
    //- Reference to the viscosity field
    volScalarField& mu_;
public:
  //- Runtime type information
  TypeName("filmViscosityModel");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
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
