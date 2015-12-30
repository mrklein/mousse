// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "centred_fit_sn_grad_scheme.hpp"
#include "linear_fit_polynomial.hpp"
#include "centred_fec_cell_to_face_stencil_object.hpp"
namespace mousse
{
  defineTemplateTypeNameAndDebug
  (
    CentredFitSnGradData<linearFitPolynomial>,
    0
  );
}
makeCentredFitSnGradScheme
(
  linearFit,
  linearFitPolynomial,
  centredFECCellToFaceStencilObject
);
