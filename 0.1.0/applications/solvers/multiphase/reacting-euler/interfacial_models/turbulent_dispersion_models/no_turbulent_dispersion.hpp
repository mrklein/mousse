// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::turbulentDispersionModels::noTurbulentDispersion
// Description
// SourceFiles
//   no_turbulent_dispersion.cpp
#ifndef no_turbulent_dispersion_hpp_
#define no_turbulent_dispersion_hpp_
#include "turbulent_dispersion_model.hpp"
namespace mousse
{
class phasePair;
namespace turbulentDispersionModels
{
class noTurbulentDispersion
:
  public turbulentDispersionModel
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from a dictionary and a phase pair
    noTurbulentDispersion
    (
      const dictionary& dict,
      const phasePair& pair
    );
  //- Destructor
  virtual ~noTurbulentDispersion();
  // Member Functions
    //- Turbulent diffusivity
    //  multiplying the gradient of the phase-fraction
    virtual tmp<volScalarField> D() const;
    //- Turbulent dispersion force
    virtual tmp<volVectorField> F() const;
};
}  // namespace turbulentDispersionModels
}  // namespace mousse
#endif
