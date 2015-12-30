// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pure_upwind_fit_scheme.hpp"
#include "linear_fit_polynomial.hpp"
#include "pure_upwind_cfc_cell_to_face_stencil_object.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug
  (
    UpwindFitData<linearFitPolynomial>,
    0
  );
  makePureUpwindFitSurfaceInterpolationScheme
  (
    linearPureUpwindFit,
    linearFitPolynomial,
    pureUpwindCFCCellToFaceStencilObject
  );
}
