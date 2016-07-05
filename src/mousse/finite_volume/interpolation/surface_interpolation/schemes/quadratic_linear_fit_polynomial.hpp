#ifndef FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_QUADRATIC_LINEAR_FIT_POLYNOMIAL_HPP_
#define FINITE_VOLUME_INTERPOLATION_SURFACE_INTERPOLATION_SCHEMES_QUADRATIC_LINEAR_FIT_POLYNOMIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::quadraticLinearFitPolynomial
// Description
//   Quadratic/linear polynomial for interpolation fitting:
//     quadratic normal to the face,
//     linear in the plane of the face for consistency with 2nd-order Gauss.
//   Can be used with the CentredFit scheme to create a quadratic surface
//   interpolation scheme

#include "vector.hpp"


namespace mousse {

class quadraticLinearFitPolynomial
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
      if (dim >= 2) {
        coeffs[curIdx++] = weight*d.y();
        coeffs[curIdx++] = weight*d.x()*d.y();
      }
      if (dim == 3) {
        coeffs[curIdx++] = weight*d.z();
        coeffs[curIdx++] = weight*d.x()*d.z();
      }
    }
};

}  // namespace mousse

#endif

