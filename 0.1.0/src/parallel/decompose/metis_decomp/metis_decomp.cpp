// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "metis_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"


extern "C"
{
  #define OMPI_SKIP_MPICXX
  #include "metis.h"
}

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(metisDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(decompositionMethod, metisDecomp, dictionary);

}


// Private Member Functions
mousse::label mousse::metisDecomp::decompose
(
  const List<label>& adjncy,
  const List<label>& xadj,
  const scalarField& cWeights,
  List<label>& finalDecomp
)
{
  // Temporal storage in terms of METIS types
  List<idx_t> t_adjncy{adjncy.size()};
  List<idx_t> t_xadj{xadj.size()};
  Field<real_t> t_cWeights{cWeights.size()};
  List<idx_t> t_finalDecomp{finalDecomp.size()};

  // Convert arguments to METIS types
  FOR_ALL(adjncy, idx) {
    t_adjncy[idx] = static_cast<idx_t>(adjncy[idx]);
  }

  FOR_ALL(xadj, idx) {
    t_xadj[idx] = static_cast<idx_t>(xadj[idx]);
  }

  FOR_ALL(cWeights, idx) {
    t_cWeights[idx] = static_cast<idx_t>(cWeights[idx]);
  }

  // Method of decomposition
  // recursive: multi-level recursive bisection (default)
  // k-way: multi-level k-way
  word method{"recursive"};
  label numCells = xadj.size() - 1;
  // decomposition options
  List<idx_t> options{METIS_NOPTIONS};
  METIS_SetDefaultOptions(options.begin());
  // processor weights initialised with no size, only used if specified in
  // a file
  Field<real_t> processorWeights;
  // cell weights (so on the vertices of the dual)
  List<idx_t> cellWeights;
  // face weights (so on the edges of the dual)
  List<idx_t> faceWeights;
  // Check for externally provided cellweights and if so initialise weights
  scalar minWeights = gMin(t_cWeights);
  if (t_cWeights.size() > 0) {
    if (minWeights <= 0) {
      WARNING_IN_FUNCTION
        << "Illegal minimum weight " << minWeights
        << endl;
    }
    if (t_cWeights.size() != numCells) {
      FATAL_ERROR_IN_FUNCTION
        << "Number of cell weights " << cWeights.size()
        << " does not equal number of cells " << numCells
        << exit(FatalError);
    }
    // Convert to integers.
    cellWeights.setSize(t_cWeights.size());
    FOR_ALL(cellWeights, i) {
      cellWeights[i] = static_cast<idx_t>(t_cWeights[i]/minWeights);
    }
  }
  // Check for user supplied weights and decomp options
  if (decompositionDict_.found("metisCoeffs")) {
    const dictionary& metisCoeffs = decompositionDict_.subDict("metisCoeffs");
    word weightsFile;
    if (metisCoeffs.readIfPresent("method", method)) {
      if (method != "recursive" && method != "k-way") {
        FATAL_ERROR_IN_FUNCTION
          << "Method " << method << " in metisCoeffs in dictionary : "
          << decompositionDict_.name()
          << " should be 'recursive' or 'k-way'"
          << exit(FatalError);
      }
      Info << "metisDecomp : Using Metis method     " << method << nl << endl;
    }
    if (metisCoeffs.readIfPresent("options", options)) {
      if (options.size() != METIS_NOPTIONS) {
        FATAL_ERROR_IN_FUNCTION
          << "Number of options in metisCoeffs in dictionary : "
          << decompositionDict_.name()
          << " should be " << METIS_NOPTIONS
          << exit(FatalError);
      }
      Info << "metisDecomp : Using Metis options     " << options
        << nl << endl;
    }
    if (metisCoeffs.readIfPresent("processorWeights", processorWeights)) {
      processorWeights /= sum(processorWeights);
      if (processorWeights.size() != nProcessors_) {
        FATAL_ERROR_IN_FUNCTION
          << "Number of processor weights "
          << processorWeights.size()
          << " does not equal number of domains " << nProcessors_
          << exit(FatalError);
      }
    }
  }
  idx_t ncon = 1;
  idx_t nProcs = nProcessors_;
  // output: cell -> processor addressing
  finalDecomp.setSize(numCells);
  t_finalDecomp.setSize(numCells);
  // output: number of cut edges
  idx_t edgeCut = 0;
  idx_t t_numCells = static_cast<idx_t>(numCells);
  if (method == "recursive") {
    METIS_PartGraphRecursive
    (
      &t_numCells,          // num vertices in graph
      &ncon,              // num balancing constraints
      const_cast<List<idx_t>&>(t_xadj).begin(),   // indexing into adjncy
      const_cast<List<idx_t>&>(t_adjncy).begin(), // neighbour info
      cellWeights.begin(),// vertexweights
      NULL,               // vsize: total communication vol
      faceWeights.begin(),// edgeweights
      &nProcs,            // nParts
      processorWeights.begin(),   // tpwgts
      NULL,               // ubvec: processor imbalance (default)
      options.begin(),
      &edgeCut,
      t_finalDecomp.begin()
    );
  } else {
    METIS_PartGraphKway
    (
      &t_numCells,         // num vertices in graph
      &ncon,              // num balancing constraints
      const_cast<List<idx_t>&>(t_xadj).begin(),   // indexing into adjncy
      const_cast<List<idx_t>&>(t_adjncy).begin(), // neighbour info
      cellWeights.begin(),// vertexweights
      NULL,               // vsize: total communication vol
      faceWeights.begin(),// edgeweights
      &nProcs,            // nParts
      processorWeights.begin(),   // tpwgts
      NULL,               // ubvec: processor imbalance (default)
      options.begin(),
      &edgeCut,
      t_finalDecomp.begin()
    );
  }

  // Copying final decomposition to the target
  FOR_ALL(t_finalDecomp, idx) {
    finalDecomp[idx] = static_cast<label>(t_finalDecomp[idx]);
  }

  return static_cast<label>(edgeCut);
}


// Constructors
mousse::metisDecomp::metisDecomp(const dictionary& decompositionDict)
:
  decompositionMethod{decompositionDict}
{}


// Member Functions
mousse::labelList mousse::metisDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  if (points.size() != mesh.nCells()) {
    FATAL_ERROR_IN_FUNCTION
      << "Can use this decomposition method only for the whole mesh"
      << endl
      << "and supply one coordinate (cellCentre) for every cell." << endl
      << "The number of coordinates " << points.size() << endl
      << "The number of cells in the mesh " << mesh.nCells()
      << exit(FatalError);
  }
  CompactListList<label> cellCells;
  calcCellCells(mesh, identity(mesh.nCells()), mesh.nCells(), false, cellCells);
  // Decompose using default weights
  labelList decomp;
  decompose(cellCells.m(), cellCells.offsets(), pointWeights, decomp);
  return decomp;
}
mousse::labelList mousse::metisDecomp::decompose
(
  const polyMesh& mesh,
  const labelList& agglom,
  const pointField& agglomPoints,
  const scalarField& agglomWeights
)
{
  if (agglom.size() != mesh.nCells()) {
    FATAL_ERROR_IN_FUNCTION
      << "Size of cell-to-coarse map " << agglom.size()
      << " differs from number of cells in mesh " << mesh.nCells()
      << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells;
  calcCellCells(mesh, agglom, agglomPoints.size(), false, cellCells);
  // Decompose using default weights
  labelList finalDecomp;
  decompose(cellCells.m(), cellCells.offsets(), agglomWeights, finalDecomp);
  // Rework back into decomposition for original mesh
  labelList fineDistribution{agglom.size()};
  FOR_ALL(fineDistribution, i) {
    fineDistribution[i] = finalDecomp[agglom[i]];
  }
  return finalDecomp;
}


mousse::labelList mousse::metisDecomp::decompose
(
  const labelListList& globalCellCells,
  const pointField& cellCentres,
  const scalarField& cellWeights
)
{
  if (cellCentres.size() != globalCellCells.size()) {
    FATAL_ERROR_IN_FUNCTION
      << "Inconsistent number of cells (" << globalCellCells.size()
      << ") and number of cell centres (" << cellCentres.size()
      << ")." << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells{globalCellCells};
  // Decompose using default weights
  labelList decomp;
  decompose(cellCells.m(), cellCells.offsets(), cellWeights, decomp);
  return decomp;
}

