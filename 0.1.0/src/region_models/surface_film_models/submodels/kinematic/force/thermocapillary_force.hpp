// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::thermocapillaryForce
// Description
//   Thermocapillary force
// SourceFiles
//   thermocapillary_force.cpp
#ifndef thermocapillary_force_hpp_
#define thermocapillary_force_hpp_
#include "force.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class thermocapillaryForce
:
  public force
{
private:
  // Private member functions
    //- Disallow default bitwise copy construct
    thermocapillaryForce(const thermocapillaryForce&);
    //- Disallow default bitwise assignment
    void operator=(const thermocapillaryForce&);
public:
  //- Runtime type information
  TypeName("thermocapillary");
  // Constructors
    //- Construct from surface film model
    thermocapillaryForce
    (
      surfaceFilmModel& owner,
      const dictionary& dict
    );
  //- Destructor
  virtual ~thermocapillaryForce();
  // Member Functions
    // Evolution
      //- Correct
      virtual tmp<fvVectorMatrix> correct(volVectorField& U);
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
