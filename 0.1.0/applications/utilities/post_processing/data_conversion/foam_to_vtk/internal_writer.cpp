// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "internal_writer.hpp"
#include "write_funs.hpp"
// Constructors 
mousse::internalWriter::internalWriter
(
  const vtkMesh& vMesh,
  const bool binary,
  const fileName& fName
)
:
  vMesh_(vMesh),
  binary_(binary),
  fName_(fName),
  os_(fName.c_str())
{
  const fvMesh& mesh = vMesh_.mesh();
  const vtkTopo& topo = vMesh_.topo();
  // Write header
  writeFuns::writeHeader(os_, binary_, mesh.time().caseName());
  os_ << "DATASET UNSTRUCTURED_GRID" << std::endl;
  // Write topology
  const labelList& addPointCellLabels = topo.addPointCellLabels();
  const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();
  os_ << "POINTS " << nTotPoints << " float" << std::endl;
  DynamicList<floatScalar> ptField{3*nTotPoints};
  writeFuns::insert(mesh.points(), ptField);
  const pointField& ctrs = mesh.cellCentres();
  FOR_ALL(addPointCellLabels, api)
  {
    writeFuns::insert(ctrs[addPointCellLabels[api]], ptField);
  }
  writeFuns::write(os_, binary_, ptField);
  //
  // Write cells
  //
  const labelListList& vtkVertLabels = topo.vertLabels();
  // Count total number of vertices referenced.
  label nFaceVerts = 0;
  FOR_ALL(vtkVertLabels, cellI)
  {
    nFaceVerts += vtkVertLabels[cellI].size() + 1;
  }
  os_ << "CELLS " << vtkVertLabels.size() << ' ' << nFaceVerts << std::endl;
  DynamicList<label> vertLabels{nFaceVerts};
  FOR_ALL(vtkVertLabels, cellI)
  {
    const labelList& vtkVerts = vtkVertLabels[cellI];
    vertLabels.append(vtkVerts.size());
    writeFuns::insert(vtkVerts, vertLabels);
  }
  writeFuns::write(os_, binary_, vertLabels);
  const labelList& vtkCellTypes = topo.cellTypes();
  os_ << "CELL_TYPES " << vtkCellTypes.size() << std::endl;
  // Make copy since writing might swap stuff.
  DynamicList<label> cellTypes{vtkCellTypes.size()};
  writeFuns::insert(vtkCellTypes, cellTypes);
  writeFuns::write(os_, binary_, cellTypes);
}
// Member Functions 
void mousse::internalWriter::writeCellIDs()
{
  const fvMesh& mesh = vMesh_.mesh();
  const vtkTopo& topo = vMesh_.topo();
  const labelList& vtkCellTypes = topo.cellTypes();
  const labelList& superCells = topo.superCells();
  // Cell ids first
  os_ << "cellID 1 " << vtkCellTypes.size() << " int" << std::endl;
  labelList cellId(vtkCellTypes.size());
  label labelI = 0;
  if (vMesh_.useSubMesh())
  {
    const labelList& cMap = vMesh_.subsetter().cellMap();
    FOR_ALL(mesh.cells(), cellI)
    {
      cellId[labelI++] = cMap[cellI];
    }
    FOR_ALL(superCells, superCellI)
    {
      label origCellI = cMap[superCells[superCellI]];
      cellId[labelI++] = origCellI;
    }
  }
  else
  {
    FOR_ALL(mesh.cells(), cellI)
    {
      cellId[labelI++] = cellI;
    }
    FOR_ALL(superCells, superCellI)
    {
      label origCellI = superCells[superCellI];
      cellId[labelI++] = origCellI;
    }
  }
  writeFuns::write(os_, binary_, cellId);
}
