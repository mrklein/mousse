// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "upwind_fit_scheme.hpp"
#include "cubic_upwind_fit_polynomial.hpp"
#include "upwind_cfc_cell_to_face_stencil_object.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug
  (
    UpwindFitData<cubicUpwindFitPolynomial>,
    0
  );
  makeUpwindFitSurfaceInterpolationScheme
  (
    cubicUpwindFit,
    cubicUpwindFitPolynomial,
    upwindCFCCellToFaceStencilObject
  );
}
