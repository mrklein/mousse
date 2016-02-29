#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_UNIFORM_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_UNIFORM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::uniform
// Description
//   Uniform/equally-weighted distribution model
// SourceFiles
//   uniform.cpp
#include "distribution_model.hpp"
namespace mousse
{
namespace distributionModels
{
class uniform
:
  public distributionModel
{
  // Private data
    //- Distribution minimum
    scalar minValue_;
    //- Distribution maximum
    scalar maxValue_;
public:
  //- Runtime type information
  TYPE_NAME("uniform");
  // Constructors
    //- Construct from components
    uniform(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    uniform(const uniform& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>(new uniform(*this));
    }
  //- Destructor
  virtual ~uniform();
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
