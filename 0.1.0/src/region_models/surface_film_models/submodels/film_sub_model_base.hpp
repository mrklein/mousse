#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_FILM_SUB_MODEL_BASE_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_FILM_SUB_MODEL_BASE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::subModelBase
// Description
//   Base class for surface film sub-models
// SourceFiles
//   film_sub_model_base.cpp
#include "surface_film_model.hpp"
#include "sub_model_base.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class filmSubModelBase
:
  public subModelBase
{
protected:
  // Protected data
    //- Reference to the owner surface film model
    surfaceFilmModel& owner_;
public:
  // Constructors
    //- Construct null
    filmSubModelBase(surfaceFilmModel& owner);
    //- Construct from owner film wihout name
    filmSubModelBase
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      const word& baseName,
      const word& modelType,
      const word& dictExt = "Coeffs"
    );
    //- Construct from owner film with name
    filmSubModelBase
    (
      const word& modelName,
      surfaceFilmModel& owner,
      const dictionary& dict,
      const word& baseName,
      const word& modelType
    );
  //- Destructor
  virtual ~filmSubModelBase();
  // Member Functions
    // Access
      //- Flag to indicate when to write a property
      virtual bool outputTime() const;
      //- Return const access to the owner surface film model
      inline const surfaceFilmModel& owner() const;
      //- Return the reference to the owner surface film model
      inline surfaceFilmModel& owner();
      template<class FilmType>
      inline const FilmType& filmType() const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
inline const surfaceFilmModel& filmSubModelBase::owner() const
{
  return owner_;
}
inline surfaceFilmModel& filmSubModelBase::owner()
{
  return owner_;
}
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#ifdef NoRepository
  #include "film_sub_model_base_templates.cpp"
#endif
#endif
