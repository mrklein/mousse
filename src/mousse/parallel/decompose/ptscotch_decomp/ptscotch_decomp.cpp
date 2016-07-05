// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ptscotch_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "time.hpp"
#include "ofstream.hpp"
#include "global_index.hpp"
#include "sub_field.hpp"


#pragma GCC diagnostic ignored "-Wold-style-cast"

extern "C"
{
  #include <stdio.h>
  #include <mpi.h>
  #include "ptscotch.h"
}

// Hack: scotch generates floating point errors so need to switch of error
//       trapping!
#ifdef __GLIBC__
#   ifndef _GNU_SOURCE
#       define _GNU_SOURCE
#   endif
#   include <fenv.h>
#endif


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(ptscotchDecomp, 0);
ADD_TO_RUN_TIME_SELECTION_TABLE(decompositionMethod, ptscotchDecomp, dictionary);

}


// Private Member Functions
void mousse::ptscotchDecomp::check(const int retVal, const char* str)
{
  if (retVal) {
    FATAL_ERROR_IN("ptscotchDecomp::decompose(..)")
      << "Call to scotch routine " << str << " failed."
      << exit(FatalError);
  }
}


// Call scotch with options from dictionary.
mousse::label mousse::ptscotchDecomp::decompose
(
  const fileName& meshPath,
  const List<label>& adjncy,
  const List<label>& xadj,
  const scalarField& cWeights,
  List<label>& finalDecomp
) const
{
  List<label> dummyAdjncy{1};
  List<label> dummyXadj{1};
  dummyXadj[0] = 0;
  return
    decompose
    (
      meshPath,
      adjncy.size(),
      (adjncy.size() ? adjncy.begin() : dummyAdjncy.begin()),
      xadj.size(),
      (xadj.size() ? xadj.begin() : dummyXadj.begin()),
      cWeights,
      finalDecomp
    );
}


// Call scotch with options from dictionary.
mousse::label mousse::ptscotchDecomp::decompose
(
  const fileName& meshPath,
  const label adjncySize,
  const label adjncy[],
  const label xadjSize,
  const label xadj[],
  const scalarField& cWeights,
  List<label>& finalDecomp
) const
{
  if (debug) {
    Pout << "ptscotchDecomp : entering with xadj:" << xadjSize << endl;
  }
  // Create temporal storage in term of Scotch types
  SCOTCH_Num* t_adjncy = new SCOTCH_Num[adjncySize];
  SCOTCH_Num* t_xadj = new SCOTCH_Num[xadjSize];

  for(label i = 0; i < adjncySize; ++i) {
    t_adjncy[i] = static_cast<SCOTCH_Num>(adjncy[i]);
  }

  for(label i = 0; i < xadjSize; ++i) {
    t_xadj[i] = static_cast<SCOTCH_Num>(xadj[i]);
  }


  // Dump graph
  if (decompositionDict_.found("scotchCoeffs")) {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    if (scotchCoeffs.lookupOrDefault("writeGraph", false)) {
      OFstream str
      {
        meshPath + "_" + mousse::name(Pstream::myProcNo()) + ".dgr"
      };
      Pout << "Dumping Scotch graph file to " << str.name() << endl
        << "Use this in combination with dgpart." << endl;
      globalIndex globalCells{xadjSize-1};
      // Distributed graph file (.grf)
      label version = 2;
      str << version << nl;
      // Number of files (procglbnbr)
      str << Pstream::nProcs();
      // My file number (procloc)
      str << ' ' << Pstream::myProcNo() << nl;
      // Total number of vertices (vertglbnbr)
      str << globalCells.size();
      // Total number of connections (edgeglbnbr)
      str << ' ' << returnReduce(xadj[xadjSize-1], sumOp<label>())
        << nl;
      // Local number of vertices (vertlocnbr)
      str << xadjSize-1;
      // Local number of connections (edgelocnbr)
      str << ' ' << xadj[xadjSize-1] << nl;
      // Numbering starts from 0
      label baseval = 0;
      // 100*hasVertlabels+10*hasEdgeWeights+1*hasVertWeighs
      str << baseval << ' ' << "000" << nl;
      for (label cellI = 0; cellI < xadjSize-1; cellI++) {
        label start = xadj[cellI];
        label end = xadj[cellI+1];
        str << end-start;
        for (label i = start; i < end; i++) {
          str << ' ' << adjncy[i];
        }
        str << nl;
      }
    }
  }
  // Strategy
  // ~~~~~~~~
  // Default.
  SCOTCH_Strat stradat;
  check(SCOTCH_stratInit(&stradat), "SCOTCH_stratInit");
  if (decompositionDict_.found("scotchCoeffs")) {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    string strategy;
    if (scotchCoeffs.readIfPresent("strategy", strategy)) {
      if (debug) {
        Info << "ptscotchDecomp : Using strategy " << strategy << endl;
      }
      SCOTCH_stratDgraphMap(&stradat, strategy.c_str());
    }
  }
  // Graph
  // ~~~~~
  List<SCOTCH_Num> velotab;
  // Check for externally provided cellweights and if so initialise weights
  scalar minWeights = gMin(cWeights);
  scalar maxWeights = gMax(cWeights);
  if (maxWeights > minWeights) {
    if (minWeights <= 0) {
      WARNING_IN
      (
        "ptscotchDecomp::decompose(..)"
      )
      << "Illegal minimum weight " << minWeights
      << endl;
    }
    if (cWeights.size() != xadjSize - 1) {
      FATAL_ERROR_IN
      (
        "ptscotchDecomp::decompose(..)"
      )
      << "Number of cell weights " << cWeights.size()
      << " does not equal number of cells " << xadjSize-1
      << exit(FatalError);
    }
  }
  scalar velotabSum = gSum(cWeights)/minWeights;
  scalar rangeScale(1.0);
  if (Pstream::master()) {
    if (velotabSum > scalar(labelMax - 1)) {
      // 0.9 factor of safety to avoid floating point round-off in
      // rangeScale tipping the subsequent sum over the integer limit.
      rangeScale = 0.9*scalar(labelMax - 1)/velotabSum;
      WARNING_IN
      (
        "ptscotchDecomp::decompose(...)"
      )
      << "Sum of weights has overflowed integer: " << velotabSum
      << ", compressing weight scale by a factor of " << rangeScale
      << endl;
    }
  }
  Pstream::scatter(rangeScale);
  if (maxWeights > minWeights) {
    if (cWeights.size()) {
      // Convert to integers.
      velotab.setSize(cWeights.size());
      FOR_ALL(velotab, i) {
        velotab[i] = int((cWeights[i]/minWeights - 1)*rangeScale) + 1;
      }
    } else {
      // Locally zero cells but not globally. Make sure we have
      // some size so .begin() does not return null pointer. Data
      // itself is never used.
      velotab.setSize(1);
      velotab[0] = 1;
    }
  }
  if (debug) {
    Pout << "SCOTCH_dgraphInit" << endl;
  }
  SCOTCH_Dgraph grafdat;
  check(SCOTCH_dgraphInit(&grafdat, MPI_COMM_WORLD), "SCOTCH_dgraphInit");
  if (debug) {
    Pout << "SCOTCH_dgraphBuild with:" << nl
      << "xadjSize-1      : " << xadjSize-1 << nl
      << "xadj            : " << uintptr_t(xadj) << nl
      << "velotab         : " << uintptr_t(velotab.begin()) << nl
      << "adjncySize      : " << adjncySize << nl
      << "adjncy          : " << uintptr_t(adjncy) << nl
      << endl;
  }
  check
    (
      SCOTCH_dgraphBuild
      (
        &grafdat,               // grafdat
        0,                      // baseval, c-style numbering
        xadjSize-1,             // vertlocnbr, nCells
        xadjSize-1,             // vertlocmax
        t_xadj,                 // vertloctab, start index per cell into
        // adjncy
        t_xadj+1,               // vendloctab, end index  ,,
        velotab.begin(),        // veloloctab, vtx weights
        NULL,                   // vlblloctab
        adjncySize,             // edgelocnbr, number of arcs
        adjncySize,             // edgelocsiz
        t_adjncy,               // edgeloctab
        NULL,                   // edgegsttab
        NULL                    // edlotab, edge weights
      ),
      "SCOTCH_dgraphBuild"
    );
  if (debug) {
    Pout << "SCOTCH_dgraphCheck" << endl;
  }
  check(SCOTCH_dgraphCheck(&grafdat), "SCOTCH_dgraphCheck");
  // Architecture
  // ~~~~~~~~~~~~
  // (fully connected network topology since using switch)
  if (debug) {
    Pout << "SCOTCH_archInit" << endl;
  }
  SCOTCH_Arch archdat;
  check(SCOTCH_archInit(&archdat), "SCOTCH_archInit");
  List<SCOTCH_Num> processorWeights;
  if (decompositionDict_.found("scotchCoeffs")) {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    scotchCoeffs.readIfPresent("processorWeights", processorWeights);
  }
  if (processorWeights.size()) {
    if (debug) {
      Info << "ptscotchDecomp : Using procesor weights " << processorWeights
        << endl;
    }
    check
      (
        SCOTCH_archCmpltw(&archdat, nProcessors_, processorWeights.begin()),
        "SCOTCH_archCmpltw"
      );
  } else {
    if (debug) {
      Pout << "SCOTCH_archCmplt" << endl;
    }
    check
      (
        SCOTCH_archCmplt(&archdat, nProcessors_),
        "SCOTCH_archCmplt"
      );
  }
  // Hack:switch off fpu error trapping
#ifdef  FE_NOMASK_ENV
  int oldExcepts =
    fedisableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif
  // Note: always provide allocated storage even if local size 0
  finalDecomp.setSize(max(1, xadjSize - 1));
  List<SCOTCH_Num> t_finalDecomp{finalDecomp.size(), 0};
  if (debug) {
    Pout << "SCOTCH_dgraphMap" << endl;
  }
  check
    (
      SCOTCH_dgraphMap
      (
        &grafdat,
        &archdat,
        &stradat,           // const SCOTCH_Strat *
        t_finalDecomp.begin() // parttab
      ),
      "SCOTCH_graphMap"
    );
#ifdef  FE_NOMASK_ENV
  feenableexcept(oldExcepts);
#endif
  if (debug) {
    Pout << "SCOTCH_dgraphExit" << endl;
  }
  // Release storage for graph
  SCOTCH_dgraphExit(&grafdat);
  // Release storage for strategy
  SCOTCH_stratExit(&stradat);
  // Release storage for network topology
  SCOTCH_archExit(&archdat);

  // Copying decomposition back to mousse-typed storage
  FOR_ALL(t_finalDecomp, idx) {
    finalDecomp[idx] = static_cast<label>(t_finalDecomp[idx]);
  }

  return 0;
}


// Constructors
mousse::ptscotchDecomp::ptscotchDecomp(const dictionary& decompositionDict)
:
  decompositionMethod{decompositionDict}
{}


// Member Functions
mousse::labelList mousse::ptscotchDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  if (points.size() != mesh.nCells()) {
    FATAL_ERROR_IN
    (
      "ptscotchDecomp::decompose(const pointField&, const scalarField&)"
    )
    << "Can use this decomposition method only for the whole mesh"
    << endl
    << "and supply one coordinate (cellCentre) for every cell." << endl
    << "The number of coordinates " << points.size() << endl
    << "The number of cells in the mesh " << mesh.nCells()
    << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells;
  calcCellCells
    (
      mesh,
      identity(mesh.nCells()),
      mesh.nCells(),
      true,
      cellCells
    );
  // Decompose using default weights
  List<label> finalDecomp;
  decompose
    (
      mesh.time().path()/mesh.name(),
      cellCells.m(),
      cellCells.offsets(),
      pointWeights,
      finalDecomp
    );
  // Copy back to labelList
  labelList decomp{points.size()};
  FOR_ALL(decomp, i) {
    decomp[i] = finalDecomp[i];
  }
  return decomp;
}


mousse::labelList mousse::ptscotchDecomp::decompose
(
  const polyMesh& mesh,
  const labelList& agglom,
  const pointField& agglomPoints,
  const scalarField& pointWeights
)
{
  if (agglom.size() != mesh.nCells()) {
    FATAL_ERROR_IN
    (
      "ptscotchDecomp::decompose(const labelList&, const pointField&)"
    )
    << "Size of cell-to-coarse map " << agglom.size()
    << " differs from number of cells in mesh " << mesh.nCells()
    << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells;
  calcCellCells
    (
      mesh,
      agglom,
      agglomPoints.size(),
      true,
      cellCells
    );
  // Decompose using weights
  List<label> finalDecomp;
  decompose
    (
      mesh.time().path()/mesh.name(),
      cellCells.m(),
      cellCells.offsets(),
      pointWeights,
      finalDecomp
    );
  // Rework back into decomposition for original mesh
  labelList fineDistribution{agglom.size()};
  FOR_ALL(fineDistribution, i) {
    fineDistribution[i] = finalDecomp[agglom[i]];
  }
  return fineDistribution;
}


mousse::labelList mousse::ptscotchDecomp::decompose
(
  const labelListList& globalCellCells,
  const pointField& cellCentres,
  const scalarField& cWeights
)
{
  if (cellCentres.size() != globalCellCells.size()) {
    FATAL_ERROR_IN
    (
      "ptscotchDecomp::decompose(const pointField&, const labelListList&)"
    )
    << "Inconsistent number of cells (" << globalCellCells.size()
    << ") and number of cell centres (" << cellCentres.size()
    << ")." << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells{globalCellCells};
  // Decompose using weights
  List<label> finalDecomp;
  decompose
    (
      "ptscotch",
      cellCells.m(),
      cellCells.offsets(),
      cWeights,
      finalDecomp
    );
  // Copy back to labelList
  labelList decomp{cellCentres.size()};
  FOR_ALL(decomp, i) {
    decomp[i] = finalDecomp[i];
  }
  return decomp;
}

