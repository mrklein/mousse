// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::solidification
// Description
//   Solidification phase change model where all film mass is converted when the
//   local temperature > activation temperature.  The latent heat is
//   assumed to be removed by heat-transfer to the wall.
// SourceFiles
//   solidification.cpp
#ifndef solidification_hpp_
#define solidification_hpp_
#include "phase_change_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class solidification
:
  public phaseChangeModel
{
  // Private member functions
    //- Disallow default bitwise copy construct
    solidification(const solidification&);
    //- Disallow default bitwise assignment
    void operator=(const solidification&);
protected:
  // Protected data
    //- Temperature at which solidification starts
    scalar T0_;
    //- Solidification limiter
    //  Maximum fraction of film which can solidify in a time-step
    scalar maxSolidificationFrac_;
    //- Solidification limiter
    //  Maximum rate at which the film can solidify
    dimensionedScalar maxSolidificationRate_;
    //- Accumulated solid mass [kg]
    volScalarField mass_;
    //- Accumulated solid thickness [m]
    volScalarField thickness_;
public:
  //- Runtime type information
  TypeName("solidification");
  // Constructors
    //- Construct from surface film model
    solidification(surfaceFilmModel& owner, const dictionary& dict);
  //- Destructor
  virtual ~solidification();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correctModel
      (
        const scalar dt,
        scalarField& availableMass,
        scalarField& dMass,
        scalarField& dEnergy
      );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
