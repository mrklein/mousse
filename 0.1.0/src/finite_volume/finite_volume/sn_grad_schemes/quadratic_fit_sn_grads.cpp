// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_sn_grad_scheme.hpp"
#include "quadratic_fit_polynomial.hpp"
#include "centred_cfc_cell_to_face_stencil_object.hpp"

namespace mousse
{

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  CentredFitSnGradData<quadraticFitPolynomial>,
  0
);

}

MAKE_CENTRED_FIT_SN_GRAD_SCHEME
(
  quadraticFit,
  quadraticFitPolynomial,
  centredCFCCellToFaceStencilObject
);
