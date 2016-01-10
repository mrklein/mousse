// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::normal
// Description
//   A normal distribution model
//   \verbatim
//     model = strength * exp(-0.5*((x - expectation)/variance)^2 )
//   \endverbatim
//   strength only has meaning if there's more than one distribution model
// SourceFiles
//   normal.cpp
#ifndef normal_hpp_
#define normal_hpp_
#include "distribution_model.hpp"
namespace mousse
{
namespace distributionModels
{
class normal
:
  public distributionModel
{
  // Private data
    //- Distribution minimum
    scalar minValue_;
    //- Distribution maximum
    scalar maxValue_;
    // Model coefficients
      scalar expectation_;
      scalar variance_;
      scalar a_;
public:
  //- Runtime type information
  TYPE_NAME("normal");
  // Constructors
    //- Construct from components
    normal(const dictionary& dict, cachedRandom& rndGen);
    //- Construct copy
    normal(const normal& p);
    //- Construct and return a clone
    virtual autoPtr<distributionModel> clone() const
    {
      return autoPtr<distributionModel>(new normal(*this));
    }
  //- Destructor
  virtual ~normal();
  // Member Functions
    //- Sample the distributionModel
    virtual scalar sample() const;
    //- Return the minimum value
    virtual scalar minValue() const;
    //- Return the maximum value
    virtual scalar maxValue() const;
    //- Return the mean value
    virtual scalar meanValue() const;
    virtual scalar erfInv(const scalar y) const;
};
}  // namespace distributionModels
}  // namespace mousse
#endif
