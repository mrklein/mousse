// mousse: CFD toolbox
// Copyright (C) 2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::splineInterpolationWeights
// Description
//   Catmull-Rom spline interpolation.
// SourceFiles
//   spline_interpolation_weights.cpp

#ifndef spline_interpolation_weights_hpp_
#define spline_interpolation_weights_hpp_

#include "interpolation_weights.hpp"

namespace mousse
{
class splineInterpolationWeights
:
  public interpolationWeights
{
private:
  // Private data
    //- Cached index in samples from previous invocation
    mutable label index_;
public:
  //- Runtime type information
  TYPE_NAME("spline");
  // Constructors
    //- Construct from components. By default make sure samples are
    //  equidistant.
    splineInterpolationWeights
    (
      const scalarField& samples,
      const bool checkEqualDistance = true
    );
  //- Destructor
  virtual ~splineInterpolationWeights()
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
      const scalar,
      const scalar,
      labelList& /*indices*/,
      scalarField& /*weights*/
    ) const
    {
      NOT_IMPLEMENTED
      (
        "splineInterpolationWeights::integrationWeights(..)"
      );
      return false;
    }
};
}  // namespace mousse
#endif
