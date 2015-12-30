// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regionModels::surfaceFilmModels::laminar
// Description
//   Film laminar turbulence model.
// SourceFiles
//   laminar.cpp
#ifndef laminar_hpp_
#define laminar_hpp_
#include "film_turbulence_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class laminar
:
  public filmTurbulenceModel
{
private:
  // Private Data
    //- Surface roughness coefficient
    scalar Cf_;
  // Private member functions
    //- Disallow default bitwise copy construct
    laminar(const laminar&);
    //- Disallow default bitwise assignment
    void operator=(const laminar&);
public:
  //- Runtime type information
  TypeName("laminar");
  // Constructors
    //- Construct from surface film model
    laminar(surfaceFilmModel& owner, const dictionary& dict);
  //- Destructor
  virtual ~laminar();
  // Member Functions
    // Evolution
      //- Return the film surface velocity
      virtual tmp<volVectorField> Us() const;
      //- Return the film turbulence viscosity
      virtual tmp<volScalarField> mut() const;
      //- Correct/update the model
      virtual void correct();
      //- Return the source for the film momentum equation
      virtual tmp<fvVectorMatrix> Su(volVectorField& U) const;
};
}  // namespace surfaceFilmModels
}  // namespace regionModels
}  // namespace mousse
#endif
