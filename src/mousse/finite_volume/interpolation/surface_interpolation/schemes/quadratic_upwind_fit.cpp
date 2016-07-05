// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upwind_fit_scheme.hpp"
#include "quadratic_upwind_fit_polynomial.hpp"
#include "upwind_fec_cell_to_face_stencil_object.hpp"


namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  UpwindFitData<quadraticUpwindFitPolynomial>,
  0
);

MAKE_UPWIND_FIT_SURFACE_INTERPOLATION_SCHEME
(
  quadraticUpwindFit,
  quadraticUpwindFitPolynomial,
  upwindFECCellToFaceStencilObject
);

}

