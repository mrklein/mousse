// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pure_upwind_fit_scheme.hpp"
#include "quadratic_linear_upwind_fit_polynomial.hpp"
#include "upwind_cfc_cell_to_face_stencil_object.hpp"
namespace mousse
{
  // Use stencil with three upwind cells:
  // upwindCFCCellToFaceStencilObject + pureUpwind
  makePureUpwindFitSurfaceInterpolationScheme
  (
    quadraticLinearPureUpwindFit,
    quadraticLinearUpwindFitPolynomial,
    upwindCFCCellToFaceStencilObject
  );
}
