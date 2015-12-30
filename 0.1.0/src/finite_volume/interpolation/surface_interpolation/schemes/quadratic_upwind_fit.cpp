// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upwind_fit_scheme.hpp"
#include "quadratic_upwind_fit_polynomial.hpp"
#include "upwind_fec_cell_to_face_stencil_object.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug
  (
    UpwindFitData<quadraticUpwindFitPolynomial>,
    0
  );
  makeUpwindFitSurfaceInterpolationScheme
  (
    quadraticUpwindFit,
    quadraticUpwindFitPolynomial,
    upwindFECCellToFaceStencilObject
  );
}
