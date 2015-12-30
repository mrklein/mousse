// mousse: CFD toolbox
// Copyright (C) 2013-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_least_squares_grad.hpp"
//#include "centred_cfc_cell_to_cell_stencil_object.hpp"
#include "centred_cpc_cell_to_cell_stencil_object.hpp"
#include "centred_cec_cell_to_cell_stencil_object.hpp"
// makeLeastSquaresGradScheme
// (
//     faceCellsLeastSquares,
//     centredCFCCellToCellStencilObject
// )
makeLeastSquaresGradScheme
(
  pointCellsLeastSquares,
  centredCPCCellToCellStencilObject
)
makeLeastSquaresGradScheme
(
  edgeCellsLeastSquares,
  centredCECCellToCellStencilObject
)
