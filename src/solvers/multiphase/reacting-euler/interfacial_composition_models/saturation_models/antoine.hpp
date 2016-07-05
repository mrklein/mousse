#ifndef SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_ANTOINE_HPP_
#define SOLVERS_MULTIPHASE_REACTING_EULER_INTERFACIAL_COMPOSITION_MODELS_SATURATION_MODELS_ANTOINE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::saturationModels::Antoine
// Description
//   Antoine equation for the vapour pressure.
//   \f[
//     \log p = A + \frac{B}{C + T}
//   \f]
//   Coefficients \f$A\f$, \f$B\f$ and \f$C\f$ are to be supplied and should be
//   suitable for natural logarithms and temperatures in Kelvin.
// SourceFiles
//   antoine.cpp
#include "saturation_model.hpp"
namespace mousse
{
namespace saturationModels
{
class Antoine
:
  public saturationModel
{
protected:
  // Private data
    //- Constant A
    dimensionedScalar A_;
    //- Constant B
    dimensionedScalar B_;
    //- Constant C
    dimensionedScalar C_;
public:
  //- Runtime type information
  TYPE_NAME("Antoine");
  // Constructors
    //- Construct from a dictionary
    Antoine(const dictionary& dict);
  //- Destructor
  virtual ~Antoine();
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
