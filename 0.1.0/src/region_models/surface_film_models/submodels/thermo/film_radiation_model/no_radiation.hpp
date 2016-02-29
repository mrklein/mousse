#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_NO_RADIATION_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_NO_RADIATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noRadiation
// Description
//   Dummy radiation model for 'none' option
// SourceFiles
//   no_radiation.cpp
#include "film_radiation_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class noRadiation
:
  public filmRadiationModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from surface film model and dictionary
    noRadiation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    noRadiation(const noRadiation&) = delete;
    //- Disallow default bitwise assignment
    noRadiation& operator=(const noRadiation&) = delete;
  //- Destructor
  virtual ~noRadiation();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct();
      //- Return the radiation sensible enthalpy source
      virtual tmp<volScalarField> Shs();
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
