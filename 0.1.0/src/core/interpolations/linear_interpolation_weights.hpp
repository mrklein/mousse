// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearInterpolationWeights
// Description
// SourceFiles
//   linear_interpolation_weights.cpp

#ifndef linear_interpolation_weights_hpp_
#define linear_interpolation_weights_hpp_

#include "interpolation_weights.hpp"

namespace mousse
{
class linearInterpolationWeights
:
  public interpolationWeights
{
private:
  // Private data
    //- Cached index in samples from previous invocation
    mutable label index_;
  // Private Member Functions
    //- Get weights of i and i+1 to calculate integration from t to
    //  samples_[i+1]
    Pair<scalar> integrationWeights
    (
      const label i,
      const scalar t
    ) const;
public:
  //- Runtime type information
  TYPE_NAME("linear");
  // Constructors
    //- Construct from components
    linearInterpolationWeights
    (
      const scalarField& samples
    );
  //- Destructor
  virtual ~linearInterpolationWeights()
  {}
  // Member Functions
    //- Calculate weights and indices to calculate t from samples.
    //  Returns true if indices changed.
    virtual bool valueWeights
    (
      const scalar t,
      labelList& indices,
      scalarField& weights
    ) const;
    //- Calculate weights and indices to calculate integrand of t1..t2
    //  from samples. Returns true if indices changed.
    virtual bool integrationWeights
    (
      const scalar t1,
      const scalar t2,
      labelList& indices,
      scalarField& weights
    ) const;
};
}  // namespace mousse
#endif
