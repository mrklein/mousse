#ifndef LAGRANGIAN_DISTRIBUTION_MODELS_DISTRIBUTION_MODEL_HPP_
#define LAGRANGIAN_DISTRIBUTION_MODELS_DISTRIBUTION_MODEL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::distributionModel
// Description
//   A library of runtime-selectable distribution models.
//   Returns a sampled value given the expectation (nu) and variance (sigma^2)
//   Current distribution models include:
//   - exponential
//   - fixedValue
//   - general
//   - multi-normal
//   - normal
//   - Rosin-Rammler
//   - uniform
//   The distributionModel is tabulated in equidistant nPoints, in an interval.
//   These values are integrated to obtain the cumulated distribution model,
//   which is then used to change the distribution from unifrom to
//   the actual distributionModel.
// SourceFiles
//   distribution_model.cpp
//   distribution_model_new.cpp
#include "iodictionary.hpp"
#include "auto_ptr.hpp"
#include "cached_random.hpp"
namespace mousse
{
namespace distributionModels
{
class distributionModel
{
protected:
  // Protected data
    //- Coefficients dictionary
    const dictionary distributionModelDict_;
    //- Reference to the random number generator
    cachedRandom& rndGen_;
  // Protected Member Functions
    //- Check that the distribution model is valid
    virtual void check() const;
public:
  //-Runtime type information
  TYPE_NAME("distributionModel");
  //- Declare runtime constructor selection table
  DECLARE_RUN_TIME_SELECTION_TABLE
  (
    autoPtr,
    distributionModel,
    dictionary,
    (
      const dictionary& dict,
      cachedRandom& rndGen
    ),
    (dict, rndGen)
  );
  // Constructors
    //- Construct from dictionary
    distributionModel
    (
      const word& name,
      const dictionary& dict,
      cachedRandom& rndGen
    );
    //- Construct copy
    distributionModel(const distributionModel& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const = 0;
  //- Selector
  static autoPtr<distributionModel> New
  (
    const dictionary& dict,
    cachedRandom& rndGen
  );
  //- Destructor
  virtual ~distributionModel();
  // Member Functions
    //- Sample the distributionModel
    virtual scalar sample() const = 0;
    //- Return the minimum value
    virtual scalar minValue() const = 0;
    //- Return the maximum value
    virtual scalar maxValue() const = 0;
    //- Return the maximum value
    virtual scalar meanValue() const = 0;
};
}  // namespace distributionModels
}  // namespace mousse
#endif
