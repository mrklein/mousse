// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::saturationModels::AntoineExtended
// Description
//   Extended Antoine equation for the vapour pressure.
//   \f[
//     \log (p) = A + \frac{B}{C + T} + D \log (T) + E T^F
//   \f]
//   Coefficients \f$A\f$, \f$B\f$, \f$C\f$, \f$D\f$, \f$E\f$ and \f$F\f$ are
//   to be supplied and should be suitable for natural logarithms and
//   temperatures in Kelvin.
// SourceFiles
//   antoine_extended.cpp
#ifndef antoine_extended_hpp_
#define antoine_extended_hpp_
#include "antoine.hpp"
namespace mousse
{
namespace saturationModels
{
class AntoineExtended
:
  public Antoine
{
private:
  // Private data
    //- Constant D
    dimensionedScalar D_;
    //- Constant F
    dimensionedScalar F_;
    //- Constant E
    //  (after F so F's dimensions can be used in the construction)
    dimensionedScalar E_;
public:
  //- Runtime type information
  TYPE_NAME("AntoineExtended");
  // Constructors
    //- Construct from a dictionary
    AntoineExtended(const dictionary& dict);
  //- Destructor
  virtual ~AntoineExtended();
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
