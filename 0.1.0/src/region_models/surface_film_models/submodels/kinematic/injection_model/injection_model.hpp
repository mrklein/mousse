// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::surfaceFilmModels::injectionModel
// Description
//   Base class for film injection models, handling mass transfer from the
//   film.
// SourceFiles
//   injection_model.cpp
//   injection_model_new.cpp
#ifndef injection_model_hpp_
#define injection_model_hpp_
#include "film_sub_model_base.hpp"
#include "run_time_selection_tables.hpp"
#include "scalar_field.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class injectionModel
:
  public filmSubModelBase
{
  // Private data
    //- Injected mass
    scalar injectedMass_;
protected:
  // Protected Member Functions
    //- Add to injected mass
    void addToInjectedMass(const scalar dMass);
    //- Correct
    void correct();
public:
  //- Runtime type information
  TYPE_NAME("injectionModel");
  // Declare runtime constructor selection table
    DECLARE_RUN_TIME_SELECTION_TABLE
    (
      autoPtr,
      injectionModel,
      dictionary,
      (
        surfaceFilmModel& owner,
        const dictionary& dict
      ),
      (owner, dict)
    );
  // Constructors
    //- Construct null
    injectionModel(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    injectionModel
    (
      const word& modelType,
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    injectionModel(const injectionModel&) = delete;
    //- Disallow default bitwise assignment
    injectionModel& operator=(const injectionModel&) = delete;
  // Selectors
    //- Return a reference to the selected injection model
    static autoPtr<injectionModel> New
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      const word& mdoelType
    );
  //- Destructor
  virtual ~injectionModel();
  // Member Functions
    //- Correct
    virtual void correct
    (
      scalarField& availableMass,
      scalarField& massToInject,
      scalarField& diameterToInject
    ) = 0;
    //- Return the total mass injected
    virtual scalar injectedMassTotal() const;
    //- Accumulate the total mass injected for the patches into the
    //  scalarField provided
    virtual void patchInjectedMassTotals(scalarField& /*patchMasses*/) const
    {}
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
