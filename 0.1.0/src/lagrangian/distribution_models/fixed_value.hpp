// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fixedValue
// Description
//   Returns a fixed value
// SourceFiles
//   fixed_value.cpp
#ifndef distributionModelFixedValue_H
#define distributionModelFixedValue_H
#include "distribution_model.hpp"
namespace mousse
{
namespace distributionModels
{
class fixedValue
:
  public distributionModel
{
  // Private data
    //- Fixed value
    scalar value_;
public:
  //- Runtime type information
  TypeName("fixedValue");
  // Constructors
    //- Construct from components
    fixedValue(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    fixedValue(const fixedValue& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>(new fixedValue(*this));
    }
  //- Destructor
  virtual ~fixedValue();
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
