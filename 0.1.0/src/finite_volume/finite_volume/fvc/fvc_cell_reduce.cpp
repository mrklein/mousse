// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "fvc_cell_reduce.hpp"
#include "fv_mesh.hpp"
#include "vol_fields.hpp"
#include "surface_fields.hpp"
#include "zero_gradient_fv_patch_fields.hpp"
namespace mousse
{
namespace fvc
{
template<class Type, class CombineOp>
tmp<GeometricField<Type, fvPatchField, volMesh> > cellReduce
(
  const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf,
  const CombineOp& cop
)
{
  typedef GeometricField<Type, fvPatchField, volMesh> volFieldType;
  const fvMesh& mesh = ssf.mesh();
  tmp<volFieldType> tresult
  (
    new volFieldType
    (
      IOobject
      (
        "cellReduce(" + ssf.name() + ')',
        ssf.instance(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
      ),
      mesh,
      dimensioned<Type>("0", ssf.dimensions(), pTraits<Type>::zero),
      zeroGradientFvPatchField<Type>::typeName
    )
  );
  volFieldType& result = tresult();
  const labelUList& own = mesh.owner();
  const labelUList& nbr = mesh.neighbour();
  forAll(own, i)
  {
    label cellI = own[i];
    cop(result[cellI], ssf[i]);
  }
  forAll(nbr, i)
  {
    label cellI = nbr[i];
    cop(result[cellI], ssf[i]);
  }
  result.correctBoundaryConditions();
  return tresult;
}
template<class Type, class CombineOp>
tmp<GeometricField<Type, fvPatchField, volMesh> > cellReduce
(
  const tmp<GeometricField<Type, fvsPatchField, surfaceMesh>&> tssf,
  const CombineOp& cop
)
{
  tmp<GeometricField<Type, fvPatchField, volMesh> >
    tvf(cellReduce(cop, tssf));
 tssf.clear();
  return tvf;
}
}  // namespace fvc
}  // namespace mousse
