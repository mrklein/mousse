// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::constantViscosity
// Description
//   Constant viscosity model
// SourceFiles
//   constant_viscosity.cpp
#ifndef constant_viscosity_hpp_
#define constant_viscosity_hpp_
#include "film_viscosity_model.hpp"
namespace mousse
{
namespace regionModels
{
namespace surfaceFilmModels
{
class constantViscosity
:
  public filmViscosityModel
{
  // Private member functions
    //- Disallow default bitwise copy construct
    constantViscosity(const constantViscosity&);
    //- Disallow default bitwise assignment
    void operator=(const constantViscosity&);
protected:
  // Protected data
    //- Constant viscosity [Pa.s]
    dimensionedScalar mu0_;
public:
  //- Runtime type information
  TypeName("constant");
  // Constructors
    //- Construct from surface film model
    constantViscosity
    (
      surfaceFilmModel& owner,
      const dictionary& dict,
      volScalarField& mu
    );
  //- Destructor
  virtual ~constantViscosity();
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
