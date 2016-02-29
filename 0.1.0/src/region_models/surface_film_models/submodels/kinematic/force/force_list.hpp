#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_FORCE_LIST_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_KINEMATIC_FORCE_FORCE_LIST_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::forceList
// Description
//   List container for film sources
// SourceFiles
//   force_list.cpp
#include "ptr_list.hpp"
#include "force.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class forceList
:
  public PtrList<force>
{
public:
  // Constructors
    //- Construct null
    forceList(surfaceFilmModel& owner);
    //- Construct from type name, dictionary and surface film model
    forceList
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~forceList();
  // Member functions
    //- Return (net) force system
    tmp<fvVectorMatrix> correct(volVectorField& U);
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
