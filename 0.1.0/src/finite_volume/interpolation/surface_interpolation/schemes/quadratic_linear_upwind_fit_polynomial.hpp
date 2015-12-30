// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::quadraticLinearUpwindFitPolynomial
// Description
//   Quadratic polynomial for upwind biased interpolation fitting.
//   Can be used with the UpwindFit scheme to create a quadratic surface
//   interpolation scheme
#ifndef quadratic_linear_upwind_fit_polynomial_hpp_
#define quadratic_linear_upwind_fit_polynomial_hpp_
#include "vector.hpp"
namespace mousse
{
class quadraticLinearUpwindFitPolynomial
{
public:
  // Member functions
    static label nTerms(const direction dim)
    {
      return
      (
        dim == 1 ? 3 :
        dim == 2 ? 5 :
        dim == 3 ? 7 : 0
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
      coeffs[curIdx++] = weight*sqr(d.x());
      if (dim >= 2)
      {
        coeffs[curIdx++] = weight*d.y();
        coeffs[curIdx++] = weight*d.x()*d.y();
      }
      if (dim == 3)
      {
        coeffs[curIdx++] = weight*d.z();
        coeffs[curIdx++] = weight*d.x()*d.z();
      }
    }
};
}  // namespace mousse
#endif
