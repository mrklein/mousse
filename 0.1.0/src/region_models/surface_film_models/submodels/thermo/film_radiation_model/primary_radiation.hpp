// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::primaryRadiation
// Description
//   Radiation model whereby the radiative heat flux is mapped from the primary
//   region
// SourceFiles
//   primary_radiation.cpp
#ifndef primary_radiation_hpp_
#define primary_radiation_hpp_
#include "film_radiation_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class primaryRadiation
:
  public filmRadiationModel
{
private:
  // Private data
    //- Incident radiative flux mapped from  the primary region / [kg/s3]
    volScalarField QinPrimary_;
public:
  //- Runtime type information
  TYPE_NAME("primaryRadiation");
  // Constructors
    //- Construct from surface film model and dictionary
    primaryRadiation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    primaryRadiation(const primaryRadiation&) = delete;
    //- Disallow default bitwise assignment
    primaryRadiation& operator=(const primaryRadiation&) = delete;
  //- Destructor
  virtual ~primaryRadiation();
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
