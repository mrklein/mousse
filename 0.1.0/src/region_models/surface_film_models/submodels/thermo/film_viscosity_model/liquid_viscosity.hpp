// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::liquidViscosity
// Description
//   liquidViscosity viscosity model
// SourceFiles
//   liquid_viscosity.cpp
#ifndef liquid_viscosity_hpp_
#define liquid_viscosity_hpp_
#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class liquidViscosity
:
  public filmViscosityModel
{
private:
  // Private member functions
    //- Disallow default bitwise copy construct
    liquidViscosity(const liquidViscosity&);
    //- Disallow default bitwise assignment
    void operator=(const liquidViscosity&);
protected:
  // Protected data
public:
  //- Runtime type information
  TypeName("liquid");
  // Constructors
    //- Construct from surface film model
    liquidViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
  //- Destructor
  virtual ~liquidViscosity();
  // Member Functions
    // Evolution
      //- Correct
      virtual void correct
      (
        const volScalarField& p,
        const volScalarField& T
      );
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
