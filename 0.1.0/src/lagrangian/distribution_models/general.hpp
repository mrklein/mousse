#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_GENERAL_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_GENERAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::general
// Description
//   general distribution model
// SourceFiles
//   general.cpp
#include "distribution_model.hpp"
#include "vector.hpp"
#include "vector_space.hpp"
namespace mousse
{
namespace distributionModels
{
class general
:
  public distributionModel
{
  // Private data
    typedef VectorSpace<Vector<scalar>, scalar, 2> pair;
    List<pair> xy_;
    label nEntries_;
    //- Min and max values of the distribution
    scalar minValue_;
    scalar maxValue_;
    scalar meanValue_;
    List<scalar> integral_;
public:
  //- Runtime type information
  TYPE_NAME("general");
  // Constructors
    //- Construct from components
    general(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    general(const general& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>(new general(*this));
    }
  //- Destructor
  virtual ~general();
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
