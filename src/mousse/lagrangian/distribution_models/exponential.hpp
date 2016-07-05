#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_EXPONENTIAL_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_EXPONENTIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::exponential
// Description
//   exponential distribution model

#include "distribution_model.hpp"


namespace mousse {
namespace distributionModels {

class exponential
:
  public distributionModel
{
  // Private data
    //- Distribution minimum
    scalar minValue_;
    //- Distribution maximum
    scalar maxValue_;
    // Model coefficients
      scalar lambda_;
public:
  //- Runtime type information
  TYPE_NAME("exponential");
  // Constructors
    //- Construct from components
    exponential(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    exponential(const exponential& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>{new exponential{*this}};
    }
  //- Destructor
  virtual ~exponential();
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

