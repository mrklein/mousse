// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
//   fvc_cell_reduce.cpp
#ifndef fvc_cell_reduce_hpp_
#define fvc_cell_reduce_hpp_
#include "vol_fields_fwd.hpp"
#include "surface_fields_fwd.hpp"
namespace mousse
{
namespace fvc
{
  template<class Type, class CombineOp>
  tmp<GeometricField<Type, fvPatchField, volMesh> > cellReduce
  (
    const GeometricField<Type, fvsPatchField, surfaceMesh>&,
    const CombineOp& cop
  );
  template<class Type, class CombineOp>
  tmp<GeometricField<Type, fvPatchField, volMesh> > cellReduce
  (
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&,
    const CombineOp& cop
  );
}
}  // namespace mousse
#ifdef NoRepository
#   include "fvc_cell_reduce.cpp"
#endif
#endif
