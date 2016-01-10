// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_scheme.hpp"
#include "bi_linear_fit_polynomial.hpp"
#include "centred_cfc_cell_to_face_stencil_object.hpp"
namespace mousse
{

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  CentredFitData<biLinearFitPolynomial>,
  0
);

MAKE_CENTRED_FIT_SURFACE_INTERPOLATION_SCHEME
(
  biLinearFit,
  biLinearFitPolynomial,
  centredCFCCellToFaceStencilObject
);

}
