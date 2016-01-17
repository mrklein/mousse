// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::reactionRateFlameAreaModels::consumptionSpeed
// Description
//   Correlation function for laminar consumption speed obtained from flamelet
//   solution at increasing strain rates.
// SourceFiles
//   consumption_speed.cpp
#ifndef consumption_speed_hpp_
#define consumption_speed_hpp_
#include "iodictionary.hpp"
#include "vol_fields.hpp"
namespace mousse
{
class consumptionSpeed
{
  // Private Data
    //- Maximum consumption speed
    scalar omega0_;
    //- Exponential factor
    scalar eta_;
    //- Extinction strain
    scalar sigmaExt_;
    //- Minimum consumption speed
    scalar omegaMin_;
  // Private member functions
    //- Return consumption rate
    scalar omega0Sigma(scalar sigma, scalar a) const;
public:
  //- Runtime type information
  TYPE_NAME("consumptionSpeed");
  // Constructors
    //- Construct from dictionary
    consumptionSpeed(const dictionary& dict);
    //- Disallow copy construct
    consumptionSpeed(const consumptionSpeed&) = delete;
    //- Disallow default bitwise assignment
    consumptionSpeed& operator=(const consumptionSpeed&) = delete;
  //- Destructor
    virtual ~consumptionSpeed();
  // Member functions
    //- Return speed consumption rate temp
    tmp<volScalarField> omega0Sigma(const volScalarField& sigma);
    // Access functions
      scalar omega0() const
      {
        return omega0_;
      }
      scalar eta() const
      {
        return eta_;
      }
      scalar sigmaExt() const
      {
        return sigmaExt_;
      }
      scalar omegaMin() const
      {
        return omegaMin_;
      }
  // IO
    //- Update properties
    void read(const dictionary& dict);
};
}  // namespace mousse
#endif
