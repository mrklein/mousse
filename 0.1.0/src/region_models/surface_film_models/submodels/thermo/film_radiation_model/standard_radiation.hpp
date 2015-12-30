// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::standardRadiation
// Description
//   Standard radiation model
// SourceFiles
//   standard_radiation.cpp
#ifndef standard_radiation_hpp_
#define standard_radiation_hpp_
#include "film_radiation_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
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
  // Private member functions
    //- Disallow default bitwise copy construct
    standardRadiation(const standardRadiation&);
    //- Disallow default bitwise assignment
    void operator=(const standardRadiation&);
public:
  //- Runtime type information
  TypeName("standardRadiation");
  // Constructors
    //- Construct from surface film model and dictionary
    standardRadiation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
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
