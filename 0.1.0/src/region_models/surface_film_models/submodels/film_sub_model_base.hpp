// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::subModelBase
// Description
//   Base class for surface film sub-models
// SourceFiles
//   film_sub_model_base_i.hpp
//   film_sub_model_base.cpp
#ifndef film_sub_model_base_hpp_
#define film_sub_model_base_hpp_
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
#include "film_sub_model_base_i.hpp"
#ifdef NoRepository
  #include "film_sub_model_base_templates.cpp"
#endif
#endif
