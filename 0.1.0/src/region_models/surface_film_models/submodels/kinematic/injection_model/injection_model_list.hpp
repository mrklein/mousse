#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_INJECTION_MODEL_INJECTION_MODEL_LIST_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_INJECTION_MODEL_INJECTION_MODEL_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::injectionModelList
// Description
//   List container for film injection models
// SourceFiles
//   injection_model_list.cpp
#include "ptr_list.hpp"
#include "injection_model.hpp"
#include "film_sub_model_base.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class injectionModelList
:
  public PtrList<injectionModel>,
  public filmSubModelBase
{
private:
  // Private data
    //- List of mass injected per patch
    scalarField massInjected_;
public:
  // Constructors
    //- Construct null
    injectionModelList(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    injectionModelList
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    injectionModelList(const injectionModelList&) = delete;
    //- Disallow default bitwise assignment
    injectionModelList& operator=(const injectionModelList&) = delete;
  //- Destructor
  virtual ~injectionModelList();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        scalarField& availableMass,
        volScalarField& massToInject,
        volScalarField& diameterToInject
      );
    // I-O
      //- Provide some info
      virtual void info(Ostream& os);
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
