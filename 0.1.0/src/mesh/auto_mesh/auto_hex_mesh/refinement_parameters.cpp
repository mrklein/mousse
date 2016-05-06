// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "refinement_parameters.hpp"
#include "unit_conversion.hpp"
#include "poly_mesh.hpp"
#include "global_index.hpp"


// Constructors 
mousse::refinementParameters::refinementParameters(const dictionary& dict)
:
  maxGlobalCells_{readLabel(dict.lookup("maxGlobalCells"))},
  maxLocalCells_{readLabel(dict.lookup("maxLocalCells"))},
  minRefineCells_{readLabel(dict.lookup("minRefinementCells"))},
  planarAngle_
  {
    dict.lookupOrDefault
    (
      "planarAngle",
      readScalar(dict.lookup("resolveFeatureAngle"))
    )
  },
  nBufferLayers_{readLabel(dict.lookup("nCellsBetweenLevels"))},
  keepPoints_{pointField(1, dict.lookup("locationInMesh"))},
  allowFreeStandingZoneFaces_{dict.lookup("allowFreeStandingZoneFaces")},
  useTopologicalSnapDetection_
  {
    dict.lookupOrDefault<bool>("useTopologicalSnapDetection", true)
  },
  maxLoadUnbalance_{dict.lookupOrDefault<scalar>("maxLoadUnbalance", 0)},
  handleSnapProblems_
  {
    dict.lookupOrDefault<Switch>("handleSnapProblems", true)
  }
{
  scalar featAngle{readScalar(dict.lookup("resolveFeatureAngle"))};
  if (featAngle < 0 || featAngle > 180) {
    curvature_ = -GREAT;
  } else {
    curvature_ = mousse::cos(degToRad(featAngle));
  }
}


// Member Functions 
mousse::labelList mousse::refinementParameters::findCells(const polyMesh& mesh)
const
{
  // Force calculation of tet-diag decomposition (for use in findCell)
  (void)mesh.tetBasePtIs();
  // Global calculation engine
  globalIndex globalCells{mesh.nCells()};
  // Cell label per point
  labelList cellLabels{keepPoints_.size()};
  FOR_ALL(keepPoints_, i) {
    const point& keepPoint = keepPoints_[i];
    label localCellI = mesh.findCell(keepPoint);
    label globalCellI = -1;
    if (localCellI != -1) {
      globalCellI = globalCells.toGlobal(localCellI);
    }
    reduce(globalCellI, maxOp<label>());
    if (globalCellI == -1) {
      FATAL_ERROR_IN
      (
        "refinementParameters::findCells(const polyMesh&) const"
      )
      << "Point " << keepPoint
      << " is not inside the mesh or on a face or edge." << nl
      << "Bounding box of the mesh:" << mesh.bounds()
      << exit(FatalError);
    }
    label procI = globalCells.whichProcID(globalCellI);
    label procCellI = globalCells.toLocal(procI, globalCellI);
    Info << "Found point " << keepPoint << " in cell " << procCellI
      << " on processor " << procI << endl;
    if (globalCells.isLocal(globalCellI)) {
      cellLabels[i] = localCellI;
    } else {
      cellLabels[i] = -1;
    }
  }
  return cellLabels;
}

