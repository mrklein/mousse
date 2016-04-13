#ifndef FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_CELL_REDUCE_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FVC_FVC_CELL_REDUCE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"


namespace mousse {
namespace fvc {

template<class Type, class CombineOp>
tmp<GeometricField<Type, fvPatchField, volMesh>> cellReduce
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>&,
  const CombineOp& cop
);

template<class Type, class CombineOp>
tmp<GeometricField<Type, fvPatchField, volMesh>> cellReduce
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>>&,
  const CombineOp& cop
);

}
}  // namespace mousse

#include "fvc_cell_reduce.ipp"

#endif
