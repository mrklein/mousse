#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LINEAR_FIT_POLYNOMIAL_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_LINEAR_FIT_POLYNOMIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::linearFitPolynomial
// Description
//   Linear polynomial for interpolation fitting.
//   Can be used with the CentredFit scheme to create a linear surface
//   interpolation scheme
#include "vector.hpp"
namespace mousse
{
class linearFitPolynomial
{
public:
  // Member functions
    static label nTerms(const direction dim)
    {
      return
      (
        dim == 1 ? 2 :
        dim == 2 ? 3 :
        dim == 3 ? 4 : 0
      );
    }
    static void addCoeffs
    (
      scalar* coeffs,
      const vector& d,
      const scalar weight,
      const direction dim
    )
    {
      label curIdx = 0;
      coeffs[curIdx++] = weight;
      coeffs[curIdx++] = weight*d.x();
      if (dim >= 2)
      {
        coeffs[curIdx++] = weight*d.y();
      }
      if (dim == 3)
      {
        coeffs[curIdx++] = weight*d.z();
      }
    }
};
}  // namespace mousse
#endif
