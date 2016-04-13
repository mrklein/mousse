#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_MULTI_NORMAL_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_MULTI_NORMAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::multiNormal
// Description
//   A multiNormal distribution model
//   \verbatim
//     model = sum_i strength_i * exp(-0.5*((x - expectation_i)/variance_i)^2 )
//   \endverbatim

#include "distribution_model.hpp"


namespace mousse {
namespace distributionModels {

class multiNormal
:
  public distributionModel
{
  // Private data
    //- Distribution minimum
    scalar minValue_;
    //- Distribution maximum
    scalar maxValue_;
    //- Distribution range
    scalar range_;
    // Model coefficients
      List<scalar> expectation_;
      List<scalar> variance_;
      List<scalar> strength_;
public:
  //- Runtime type information
  TYPE_NAME("multiNormal");
  // Constructors
    //- Construct from components
    multiNormal(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    multiNormal(const multiNormal& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>{new multiNormal{*this}};
    }
  //- Destructor
  virtual ~multiNormal();
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

