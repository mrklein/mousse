#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_ROSIN_RAMMLER_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_ROSIN_RAMMLER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::RosinRammler
// Description
//   Rosin-Rammler distributionModel
//  \f[
//    cumulative model =
//      (1.0 - exp( -(( x - d0)/d)^n )
//     / (1.0 - exp( -((d1 - d0)/d)^n )
//  \f]

#include "distribution_model.hpp"


namespace mousse {
namespace distributionModels {

class RosinRammler
:
  public distributionModel
{
  // Private data
    //- Distribution minimum
    scalar minValue_;
    //- Distribution maximum
    scalar maxValue_;
    // Model coefficients
      scalar d_;
      scalar n_;
public:
  //- Runtime type information
  TYPE_NAME("RosinRammler");
  // Constructors
    //- Construct from components
    RosinRammler(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    RosinRammler(const RosinRammler& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>{new RosinRammler{*this}};
    }
  //- Destructor
  virtual ~RosinRammler();
  // Member Functions
    //- Sample the distributionModel
    virtual scalar sample() const;
    //- Return the minimum value
    virtual scalar minValue() const;
    //- Return the maximum value
    virtual scalar maxValue() const;
    //- Return the mean value
    virtual scalar meanValue() const;
};

}  // namespace distributionModels
}  // namespace mousse

#endif
