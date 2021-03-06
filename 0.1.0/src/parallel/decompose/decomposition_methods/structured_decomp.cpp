// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "structured_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "face_cell_wave.hpp"
#include "topo_distance_data.hpp"
#include "fv_mesh_subset.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(structuredDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE
(
  decompositionMethod,
  structuredDecomp,
  dictionary
);

}


// Constructors 
mousse::structuredDecomp::structuredDecomp(const dictionary& decompositionDict)
:
  decompositionMethod{decompositionDict},
  methodDict_{decompositionDict_.subDict(typeName + "Coeffs")},
  patches_{methodDict_.lookup("patches")}
{
  methodDict_.set("numberOfSubdomains", nDomains());
  method_ = decompositionMethod::New(methodDict_);
}


// Member Functions 
bool mousse::structuredDecomp::parallelAware() const
{
  return method_().parallelAware();
}


mousse::labelList mousse::structuredDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& cc,
  const scalarField& cWeights
)
{
  const polyBoundaryMesh& pbm = mesh.boundaryMesh();
  const labelHashSet patchIDs{pbm.patchSet(patches_)};
  label nFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter) {
    nFaces += pbm[iter.key()].size();
  }
  // Extract a submesh.
  labelHashSet patchCells{2*nFaces};
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter) {
    const labelUList& fc = pbm[iter.key()].faceCells();
    FOR_ALL(fc, i) {
      patchCells.insert(fc[i]);
    }
  }
  // Subset the layer of cells next to the patch
  fvMeshSubset subsetter{dynamic_cast<const fvMesh&>(mesh)};
  subsetter.setLargeCellSubset(patchCells);
  const fvMesh& subMesh = subsetter.subMesh();
  pointField subCc{cc, subsetter.cellMap()};
  scalarField subWeights{cWeights, subsetter.cellMap()};
  // Decompose the layer of cells
  labelList subDecomp{method_().decompose(subMesh, subCc, subWeights)};
  // Transfer to final decomposition
  labelList finalDecomp{cc.size(), -1};
  FOR_ALL(subDecomp, i) {
    finalDecomp[subsetter.cellMap()[i]] = subDecomp[i];
  }
  // Field on cells and faces.
  List<topoDistanceData> cellData{mesh.nCells()};
  List<topoDistanceData> faceData{mesh.nFaces()};
  // Start of changes
  labelList patchFaces{nFaces};
  List<topoDistanceData> patchData{nFaces};
  nFaces = 0;
  FOR_ALL_CONST_ITER(labelHashSet, patchIDs, iter) {
    const polyPatch& pp = pbm[iter.key()];
    const labelUList& fc = pp.faceCells();
    FOR_ALL(fc, i) {
      patchFaces[nFaces] = pp.start()+i;
      patchData[nFaces] = topoDistanceData(finalDecomp[fc[i]], 0);
      nFaces++;
    }
  }
  // Propagate information inwards
  FaceCellWave<topoDistanceData> deltaCalc
    (
      mesh,
      patchFaces,
      patchData,
      faceData,
      cellData,
      mesh.globalData().nTotalCells()+1
    );
  // And extract
  bool haveWarned = false;
  FOR_ALL(finalDecomp, cellI) {
    if (!cellData[cellI].valid(deltaCalc.data())) {
      if (!haveWarned) {
        WARNING_IN("structuredDecomp::decompose(..)")
          << "Did not visit some cells, e.g. cell " << cellI
          << " at " << mesh.cellCentres()[cellI] << endl
          << "Assigning  these cells to domain 0." << endl;
        haveWarned = true;
      }
      finalDecomp[cellI] = 0;
    } else {
      finalDecomp[cellI] = cellData[cellI].data();
    }
  }
  return finalDecomp;
}


mousse::labelList mousse::structuredDecomp::decompose
(
  const labelListList& /*globalPointPoints*/,
  const pointField& /*points*/,
  const scalarField& /*pointWeights*/
)
{
  NOT_IMPLEMENTED
  (
    "structuredDecomp::decompose\n"
    "(\n"
    "    const labelListList&,\n"
    "    const pointField&,\n"
    "    const scalarField&\n"
    ")\n"
  );
  return labelList::null();
}

