// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantRadiation
// Description
//   Film constant radiation model.  The constant radiative flux is specified
//   by the user, and operated over a time interval defined by a start time and
//   duration.  In addition, a mask can be applied to shield the film from the
//   radiation.
// SourceFiles
//   constant_radiation.cpp
#ifndef constant_radiation_hpp_
#define constant_radiation_hpp_
#include "film_radiation_model.hpp"
#include "vol_fields_fwd.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class constantRadiation
:
  public filmRadiationModel
{
private:
  // Private data
    //- Constant radiative flux [kg/s3]
    volScalarField QrConst_;
    //- Radiation mask
    volScalarField mask_;
    //- Absorptivity
    scalar absorptivity_;
    //- Time start [s]
    const scalar timeStart_;
    //- Duration [s]
    const scalar duration_;
public:
  //- Runtime type information
  TYPE_NAME("constantRadiation");
  // Constructors
    //- Construct from surface film model and dictionary
    constantRadiation
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
    //- Disallow default bitwise copy construct
    constantRadiation(const constantRadiation&) = delete;
    //- Disallow default bitwise assignment
    constantRadiation& operator=(const constantRadiation&) = delete;
  //- Destructor
  virtual ~constantRadiation();
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
