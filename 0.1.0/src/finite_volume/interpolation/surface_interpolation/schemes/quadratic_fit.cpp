// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_scheme.hpp"
#include "quadratic_fit_polynomial.hpp"
#include "centred_cfc_cell_to_face_stencil_object.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug
  (
    CentredFitData<quadraticFitPolynomial>,
    0
  );
  makeCentredFitSurfaceInterpolationScheme
  (
    quadraticFit,
    quadraticFitPolynomial,
    centredCFCCellToFaceStencilObject
  );
}
