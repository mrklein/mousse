// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "write_funs.hpp"
#include "interpolate_point_to_cell.hpp"


// Store List in dest
template<class Type>
void mousse::writeFuns::insert
(
  const List<Type>& source,
  DynamicList<floatScalar>& dest
)
{
  FOR_ALL(source, i) {
    insert(source[i], dest);
  }
}


template<class Type>
void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  const GeometricField<Type, fvPatchField, volMesh>& vvf,
  const vtkMesh& vMesh
)
{
  const fvMesh& mesh = vMesh.mesh();
  const labelList& superCells = vMesh.topo().superCells();
  label nValues = mesh.nCells() + superCells.size();
  os << vvf.name() << ' ' << pTraits<Type>::nComponents << ' '
    << nValues << " float" << std::endl;
  DynamicList<floatScalar> fField{pTraits<Type>::nComponents*nValues};
  insert(vvf.internalField(), fField);
  FOR_ALL(superCells, superCellI) {
    label origCellI = superCells[superCellI];
    insert(vvf[origCellI], fField);
  }
  write(os, binary, fField);
}


template<class Type>
void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  const GeometricField<Type, pointPatchField, pointMesh>& pvf,
  const vtkMesh& vMesh
)
{
  const fvMesh& mesh = vMesh.mesh();
  const vtkTopo& topo = vMesh.topo();
  const labelList& addPointCellLabels = topo.addPointCellLabels();
  const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();
  os << pvf.name() << ' ' << pTraits<Type>::nComponents << ' '
    << nTotPoints << " float" << std::endl;
  DynamicList<floatScalar> fField{pTraits<Type>::nComponents*nTotPoints};
  insert(pvf, fField);
  FOR_ALL(addPointCellLabels, api) {
    label origCellI = addPointCellLabels[api];
    insert(interpolatePointToCell(pvf, origCellI), fField);
  }
  write(os, binary, fField);
}


template<class Type>
void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  const GeometricField<Type, fvPatchField, volMesh>& vvf,
  const GeometricField<Type, pointPatchField, pointMesh>& pvf,
  const vtkMesh& vMesh
)
{
  const fvMesh& mesh = vMesh.mesh();
  const vtkTopo& topo = vMesh.topo();
  const labelList& addPointCellLabels = topo.addPointCellLabels();
  const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();
  os << vvf.name() << ' ' << pTraits<Type>::nComponents << ' '
    << nTotPoints << " float" << std::endl;
  DynamicList<floatScalar> fField{pTraits<Type>::nComponents*nTotPoints};
  insert(pvf, fField);
  FOR_ALL(addPointCellLabels, api) {
    label origCellI = addPointCellLabels[api];
    insert(vvf[origCellI], fField);
  }
  write(os, binary, fField);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  const PtrList<GeometricField<Type, PatchField, GeoMesh> >& flds,
  const vtkMesh& vMesh
)
{
  FOR_ALL(flds, i) {
    write(os, binary, flds[i], vMesh);
  }
}


template<class Type>
void mousse::writeFuns::write
(
  std::ostream& os,
  const bool binary,
  const volPointInterpolation& pInterp,
  const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds,
  const vtkMesh& vMesh
)
{
  FOR_ALL(flds, i) {
    write(os, binary, flds[i], pInterp.interpolate(flds[i])(), vMesh);
  }
}

