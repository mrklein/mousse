#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_CONSTANT_SATURATION_CONDITIONS_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_CONSTANT_SATURATION_CONDITIONS_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::saturationModels::constantSaturationConditions
// Description
//   Constant saturation pressure and temperature.
// SourceFiles
//   constant_saturation_conditions.cpp
#include "saturation_model.hpp"
namespace mousse
{
namespace saturationModels
{
class constantSaturationConditions
:
  public saturationModel
{
protected:
  // Private data
    //- Constant saturation pressure
    dimensionedScalar pSat_;
    //- Constant saturation temperature
    dimensionedScalar Tsat_;
public:
  //- Runtime type information
  TYPE_NAME("constant");
  // Constructors
    //- Construct from a dictionary
    constantSaturationConditions(const dictionary& dict);
  //- Destructor
  virtual ~constantSaturationConditions();
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
