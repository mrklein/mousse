// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "structured_renumber.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "topo_distance_data.hpp"
#include "fv_mesh_subset.hpp"
#include "face_cell_wave.hpp"
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(structuredRenumber, 0);
  ADD_TO_RUN_TIME_SELECTION_TABLE
  (
    renumberMethod,
    structuredRenumber,
    dictionary
  );
}
// Constructors 
mousse::structuredRenumber::structuredRenumber
(
  const dictionary& renumberDict
)
:
  renumberMethod(renumberDict),
  methodDict_(renumberDict.subDict(typeName + "Coeffs")),
  patches_(methodDict_.lookup("patches")),
  //nLayers_(readLabel(methodDict_.lookup("nLayers"))),
  depthFirst_(methodDict_.lookup("depthFirst")),
  method_(renumberMethod::New(methodDict_)),
  reverse_(methodDict_.lookup("reverse"))
{}
// Member Functions 
mousse::labelList mousse::structuredRenumber::renumber
(
  const polyMesh& mesh,
  const pointField& points
) const
{
  if (points.size() != mesh.nCells())
  {
    FATAL_ERROR_IN
    (
      "structuredDecomp::renumber(const polyMesh&, const pointField&)"
    )   << "Number of points " << points.size()
      << " should equal the number of cells " << mesh.nCells()
      << exit(FatalError);
  }
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  const labelHashSet patchIDs(pbm.patchSet(patches_));
  label nFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter)
  {
    nFaces += pbm[iter.key()].size();
  }
  // Extract a submesh.
  labelHashSet patchCells(2*nFaces);
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter)
  {
    const labelUList& fc = pbm[iter.key()].faceCells();
    FOR_ALL(fc, i)
    {
      patchCells.insert(fc[i]);
    }
  }
  label nTotalSeeds = returnReduce(patchCells.size(), sumOp<label>());
  label nTotalCells = mesh.globalData().nTotalCells();
  const label nLayers = nTotalCells/nTotalSeeds;
  Info<< type() << " : seeding " << nTotalSeeds
    << " cells on " << nLayers << " layers" << nl
    << endl;
  // Avoid subsetMesh, FaceCellWave going through proc boundaries
  bool oldParRun = Pstream::parRun();
  Pstream::parRun() = false;
  // Work array. Used here to temporarily store the original-to-ordered
  // index. Later on used to store the ordered-to-original.
  labelList orderedToOld(points.size(), -1);
  // Subset the layer of cells next to the patch
  {
    fvMeshSubset subsetter(dynamic_cast<const fvMesh&>(mesh));
    subsetter.setLargeCellSubset(patchCells);
    const fvMesh& subMesh = subsetter.subMesh();
    pointField subPoints(points, subsetter.cellMap());
    // Decompose the layer of cells
    labelList subOrder(method_().renumber(subMesh, subPoints));
    labelList subOrigToOrdered(invert(subOrder.size(), subOrder));
    // Transfer to final decomposition
    FOR_ALL(subOrder, i)
    {
      orderedToOld[subsetter.cellMap()[i]] = subOrigToOrdered[i];
    }
  }
  // Walk out.
  labelList patchFaces(nFaces);
  List<topoDistanceData> patchData(nFaces);
  nFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter)
  {
    const polyPatch& pp = pbm[iter.key()];
    const labelUList& fc = pp.faceCells();
    FOR_ALL(fc, i)
    {
      patchFaces[nFaces] = pp.start()+i;
      patchData[nFaces] = topoDistanceData
      (
        orderedToOld[fc[i]],// passive data: order of originating face
        0                   // distance: layer
      );
      nFaces++;
    }
  }
  // Field on cells and faces.
  List<topoDistanceData> cellData(mesh.nCells());
  List<topoDistanceData> faceData(mesh.nFaces());
  // Propagate information inwards
  FaceCellWave<topoDistanceData> deltaCalc
  (
    mesh,
    patchFaces,
    patchData,
    faceData,
    cellData,
    nTotalCells+1
  );
  Pstream::parRun() = oldParRun;
  // And extract.
  // Note that distance is distance from face so starts at 1.
  bool haveWarned = false;
  FOR_ALL(orderedToOld, cellI)
  {
    if (!cellData[cellI].valid(deltaCalc.data()))
    {
      if (!haveWarned)
      {
        WARNING_IN("structuredDecomp::renumber(..)")
          << "Did not visit some cells, e.g. cell " << cellI
          << " at " << mesh.cellCentres()[cellI] << endl
          << "Assigning these cells to domain 0." << endl;
        haveWarned = true;
      }
      orderedToOld[cellI] = 0;
    }
    else
    {
      label layerI = cellData[cellI].distance();
      if (depthFirst_)
      {
        orderedToOld[nLayers*cellData[cellI].data()+layerI] = cellI;
      }
      else
      {
        orderedToOld[cellData[cellI].data()+nLayers*layerI] = cellI;
      }
    }
  }
  // Return furthest away cell first
  if (reverse_)
  {
    reverse(orderedToOld);
  }
  return orderedToOld;
}
