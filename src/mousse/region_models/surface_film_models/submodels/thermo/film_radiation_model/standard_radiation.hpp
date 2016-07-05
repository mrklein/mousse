#ifndef REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_STANDARD_RADIATION_HPP_
#define REGION_MODELS_SURFACE_FILM_MODELS_SUBMODELS_THERMO_FILM_RADIATION_MODEL_STANDARD_RADIATION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::standardRadiation
// Description
//   Standard radiation model

#include "film_radiation_model.hpp"
#include "vol_fields_fwd.hpp"


namespace mousse {
namespace regionModels {
namespace surfaceFilmModels {

class standardRadiation
:
  public filmRadiationModel
{
private:
  // Private data
    //- Radiative incident flux mapped from  the primary region / [kg/s3]
    volScalarField QinPrimary_;
    //- Remaining radiative flux after removing local contribution
    volScalarField QrNet_;
    // Model coefficients
      //- Beta coefficient
      scalar beta_;
      //- Bar(kappa) coefficient
      scalar kappaBar_;
public:
  //- Runtime type information
  TYPE_NAME("standardRadiation");
  // Constructors
    //- Construct from surface film model and dictionary
    standardRadiation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    standardRadiation(const standardRadiation&) = delete;
    //- Disallow default bitwise assignment
    standardRadiation& operator=(const standardRadiation&) = delete;
  //- Destructor
  virtual ~standardRadiation();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct();
      //- Return the radiation sensible enthalpy source
      //  Also updates QrNet
      virtual tmp<volScalarField> Shs();
};

}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse

#endif

