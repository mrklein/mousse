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
namespace mousse
{
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
  // Method of decomposition
  // recursive: multi-level recursive bisection (default)
  // k-way: multi-level k-way
  word method("recursive");
  label numCells = xadj.size()-1;
  // decomposition options
  List<label> options(METIS_NOPTIONS);
  METIS_SetDefaultOptions(options.begin());
  // processor weights initialised with no size, only used if specified in
  // a file
#if REALTYPEWIDTH == 32
  Field<floatScalar> processorWeights;
#else
  Field<doubleScalar> processorWeights;
#endif
  // cell weights (so on the vertices of the dual)
  List<label> cellWeights;
  // face weights (so on the edges of the dual)
  List<label> faceWeights;
  // Check for externally provided cellweights and if so initialise weights
  scalar minWeights = gMin(cWeights);
  if (cWeights.size() > 0)
  {
    if (minWeights <= 0)
    {
      WARNING_IN_FUNCTION
        << "Illegal minimum weight " << minWeights
        << endl;
    }
    if (cWeights.size() != numCells)
    {
      FATAL_ERROR_IN_FUNCTION
        << "Number of cell weights " << cWeights.size()
        << " does not equal number of cells " << numCells
        << exit(FatalError);
    }
    // Convert to integers.
    cellWeights.setSize(cWeights.size());
    FOR_ALL(cellWeights, i)
    {
      cellWeights[i] = int(cWeights[i]/minWeights);
    }
  }
  // Check for user supplied weights and decomp options
  if (decompositionDict_.found("metisCoeffs"))
  {
    const dictionary& metisCoeffs =
      decompositionDict_.subDict("metisCoeffs");
    word weightsFile;
    if (metisCoeffs.readIfPresent("method", method))
    {
      if (method != "recursive" && method != "k-way")
      {
        FATAL_ERROR_IN_FUNCTION
          << "Method " << method << " in metisCoeffs in dictionary : "
          << decompositionDict_.name()
          << " should be 'recursive' or 'k-way'"
          << exit(FatalError);
      }
      Info<< "metisDecomp : Using Metis method     " << method
        << nl << endl;
    }
    if (metisCoeffs.readIfPresent("options", options))
    {
      if (options.size() != METIS_NOPTIONS)
      {
        FATAL_ERROR_IN_FUNCTION
          << "Number of options in metisCoeffs in dictionary : "
          << decompositionDict_.name()
          << " should be " << METIS_NOPTIONS
          << exit(FatalError);
      }
      Info<< "metisDecomp : Using Metis options     " << options
        << nl << endl;
    }
    if (metisCoeffs.readIfPresent("processorWeights", processorWeights))
    {
      processorWeights /= sum(processorWeights);
      if (processorWeights.size() != nProcessors_)
      {
        FATAL_ERROR_IN_FUNCTION
          << "Number of processor weights "
          << processorWeights.size()
          << " does not equal number of domains " << nProcessors_
          << exit(FatalError);
      }
    }
  }
  label ncon = 1;
  label nProcs = nProcessors_;
  // output: cell -> processor addressing
  finalDecomp.setSize(numCells);
  // output: number of cut edges
  label edgeCut = 0;
  if (method == "recursive")
  {
    METIS_PartGraphRecursive
    (
      &numCells,          // num vertices in graph
      &ncon,              // num balancing constraints
      const_cast<List<label>&>(xadj).begin(),   // indexing into adjncy
      const_cast<List<label>&>(adjncy).begin(), // neighbour info
      cellWeights.begin(),// vertexweights
      NULL,               // vsize: total communication vol
      faceWeights.begin(),// edgeweights
      &nProcs,            // nParts
      processorWeights.begin(),   // tpwgts
      NULL,               // ubvec: processor imbalance (default)
      options.begin(),
      &edgeCut,
      finalDecomp.begin()
    );
  }
  else
  {
    METIS_PartGraphKway
    (
      &numCells,         // num vertices in graph
      &ncon,              // num balancing constraints
      const_cast<List<label>&>(xadj).begin(),   // indexing into adjncy
      const_cast<List<label>&>(adjncy).begin(), // neighbour info
      cellWeights.begin(),// vertexweights
      NULL,               // vsize: total communication vol
      faceWeights.begin(),// edgeweights
      &nProcs,            // nParts
      processorWeights.begin(),   // tpwgts
      NULL,               // ubvec: processor imbalance (default)
      options.begin(),
      &edgeCut,
      finalDecomp.begin()
    );
  }
  return edgeCut;
}
// Constructors 
mousse::metisDecomp::metisDecomp(const dictionary& decompositionDict)
:
  decompositionMethod(decompositionDict)
{}
// Member Functions 
mousse::labelList mousse::metisDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  if (points.size() != mesh.nCells())
  {
    FATAL_ERROR_IN_FUNCTION
      << "Can use this decomposition method only for the whole mesh"
      << endl
      << "and supply one coordinate (cellCentre) for every cell." << endl
      << "The number of coordinates " << points.size() << endl
      << "The number of cells in the mesh " << mesh.nCells()
      << exit(FatalError);
  }
  CompactListList<label> cellCells;
  calcCellCells
  (
    mesh,
    identity(mesh.nCells()),
    mesh.nCells(),
    false,
    cellCells
  );
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
  if (agglom.size() != mesh.nCells())
  {
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
  labelList fineDistribution(agglom.size());
  FOR_ALL(fineDistribution, i)
  {
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
  if (cellCentres.size() != globalCellCells.size())
  {
    FATAL_ERROR_IN_FUNCTION
      << "Inconsistent number of cells (" << globalCellCells.size()
      << ") and number of cell centres (" << cellCentres.size()
      << ")." << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells(globalCellCells);
  // Decompose using default weights
  labelList decomp;
  decompose(cellCells.m(), cellCells.offsets(), cellWeights, decomp);
  return decomp;
}
