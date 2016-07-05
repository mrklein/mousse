#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_ARDEN_BUCK_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_ARDEN_BUCK_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::saturationModels::ArdenBuck
// Description
//   ArdenBuck equation for the vapour pressure of moist air.
// SourceFiles
//   arden_buck.cpp
#include "saturation_model.hpp"
namespace mousse
{
namespace saturationModels
{
class ArdenBuck
:
  public saturationModel
{
private:
  // Private Member Functions
    //- Exponent divided by the temperature
    tmp<volScalarField> xByTC(const volScalarField& TC) const;
public:
  //- Runtime type information
  TYPE_NAME("ArdenBuck");
  // Constructors
    //- Construct from a dictionary
    ArdenBuck(const dictionary& dict);
  //- Destructor
  virtual ~ArdenBuck();
  // Member Functions
    //- Saturation pressure
    virtual tmp<volScalarField> pSat(const volScalarField& T) const;
    //- Saturation pressure derivetive w.r.t. temperature
    virtual tmp<volScalarField> pSatPrime(const volScalarField& T) const;
    //- Natural log of the saturation pressure
    virtual tmp<volScalarField> lnPSat(const volScalarField& T) const;
    //- Saturation temperature
    virtual tmp<volScalarField> Tsat(const volScalarField& p) const;
};
}  // namespace saturationModels
}  // namespace mousse
#endif
