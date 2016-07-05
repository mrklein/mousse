// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_sn_grad_scheme.hpp"
#include "linear_fit_polynomial.hpp"
#include "centred_fec_cell_to_face_stencil_object.hpp"


namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG
(
  CentredFitSnGradData<linearFitPolynomial>,
  0
);

}

MAKE_CENTRED_FIT_SN_GRAD_SCHEME
(
  linearFit,
  linearFitPolynomial,
  centredFECCellToFaceStencilObject
);
