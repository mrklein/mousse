// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::surfaceFilmModels::force
// Description
//   Base class for film (stress-based) force models
// SourceFiles
//   force.cpp
//   force_new.cpp
#ifndef force_hpp_
#define force_hpp_
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
#include "fv_matrices.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class force
:
  public filmSubModelBase
{
private:
  // Private Member Functions
    //- Disallow default bitwise copy construct
    force(const force&);
    //- Disallow default bitwise assignment
    void operator=(const force&);
public:
  //- Runtime type information
  TypeName("force");
  // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
      autoPtr,
      force,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    force(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    force
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  // Selectors
    //- Return a reference to the selected force model
    static autoPtr<force> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      const word& modelType
    );
  //- Destructor
  virtual ~force();
  // Member Functions
    // Evolution
      //- Correct
      virtual tmp<fvVectorMatrix> correct(volVectorField& U) = 0;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
