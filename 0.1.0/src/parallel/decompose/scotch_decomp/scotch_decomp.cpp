// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "scotch_decomp.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "float_scalar.hpp"
#include "time.hpp"
#include "ofstream.hpp"
#include "global_index.hpp"
#include "sub_field.hpp"
extern "C"
{
#include "scotch.h"
}
// Hack: scotch generates floating point errors so need to switch of error
//       trapping!
#ifdef __GLIBC__
#   ifndef _GNU_SOURCE
#       define _GNU_SOURCE
#   endif
#   include <fenv.h>
#endif
namespace mousse
{
  defineTypeNameAndDebug(scotchDecomp, 0);
  addToRunTimeSelectionTable
  (
    decompositionMethod,
    scotchDecomp,
    dictionary
  );
}
// Private Member Functions 
void mousse::scotchDecomp::check(const int retVal, const char* str)
{
  if (retVal)
  {
    FatalErrorIn("scotchDecomp::decompose(..)")
      << "Call to scotch routine " << str << " failed."
      << exit(FatalError);
  }
}
mousse::label mousse::scotchDecomp::decompose
(
  const fileName& meshPath,
  const List<label>& adjncy,
  const List<label>& xadj,
  const scalarField& cWeights,
  List<label>& finalDecomp
)
{
  if (!Pstream::parRun())
  {
    decomposeOneProc
    (
      meshPath,
      adjncy,
      xadj,
      cWeights,
      finalDecomp
    );
  }
  else
  {
    if (debug)
    {
      Info<< "scotchDecomp : running in parallel."
        << " Decomposing all of graph on master processor." << endl;
    }
    globalIndex globalCells(xadj.size()-1);
    label nTotalConnections = returnReduce(adjncy.size(), sumOp<label>());
    // Send all to master. Use scheduled to save some storage.
    if (Pstream::master())
    {
      Field<label> allAdjncy(nTotalConnections);
      Field<label> allXadj(globalCells.size()+1);
      scalarField allWeights(globalCells.size());
      // Insert my own
      label nTotalCells = 0;
      forAll(cWeights, cellI)
      {
        allXadj[nTotalCells] = xadj[cellI];
        allWeights[nTotalCells++] = cWeights[cellI];
      }
      nTotalConnections = 0;
      forAll(adjncy, i)
      {
        allAdjncy[nTotalConnections++] = adjncy[i];
      }
      for (int slave=1; slave<Pstream::nProcs(); slave++)
      {
        IPstream fromSlave(Pstream::scheduled, slave);
        Field<label> nbrAdjncy(fromSlave);
        Field<label> nbrXadj(fromSlave);
        scalarField nbrWeights(fromSlave);
        // Append.
        //label procStart = nTotalCells;
        forAll(nbrXadj, cellI)
        {
          allXadj[nTotalCells] = nTotalConnections+nbrXadj[cellI];
          allWeights[nTotalCells++] = nbrWeights[cellI];
        }
        // No need to renumber xadj since already global.
        forAll(nbrAdjncy, i)
        {
          allAdjncy[nTotalConnections++] = nbrAdjncy[i];
        }
      }
      allXadj[nTotalCells] = nTotalConnections;
      Field<label> allFinalDecomp;
      decomposeOneProc
      (
        meshPath,
        allAdjncy,
        allXadj,
        allWeights,
        allFinalDecomp
      );
      // Send allFinalDecomp back
      for (int slave=1; slave<Pstream::nProcs(); slave++)
      {
        OPstream toSlave(Pstream::scheduled, slave);
        toSlave << SubField<label>
        (
          allFinalDecomp,
          globalCells.localSize(slave),
          globalCells.offset(slave)
        );
      }
      // Get my own part (always first)
      finalDecomp = SubField<label>
      (
        allFinalDecomp,
        globalCells.localSize()
      );
    }
    else
    {
      // Send my part of the graph (already in global numbering)
      {
        OPstream toMaster(Pstream::scheduled, Pstream::masterNo());
        toMaster<< adjncy << SubField<label>(xadj, xadj.size()-1)
          << cWeights;
      }
      // Receive back decomposition
      IPstream fromMaster(Pstream::scheduled, Pstream::masterNo());
      fromMaster >> finalDecomp;
    }
  }
  return 0;
}
// Call scotch with options from dictionary.
mousse::label mousse::scotchDecomp::decomposeOneProc
(
  const fileName& meshPath,
  const List<label>& adjncy,
  const List<label>& xadj,
  const scalarField& cWeights,
  List<label>& finalDecomp
)
{
  // Dump graph
  if (decompositionDict_.found("scotchCoeffs"))
  {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    if (scotchCoeffs.lookupOrDefault("writeGraph", false))
    {
      OFstream str(meshPath + ".grf");
      Info<< "Dumping Scotch graph file to " << str.name() << endl
        << "Use this in combination with gpart." << endl;
      label version = 0;
      str << version << nl;
      // Numer of vertices
      str << xadj.size()-1 << ' ' << adjncy.size() << nl;
      // Numbering starts from 0
      label baseval = 0;
      // Has weights?
      label hasEdgeWeights = 0;
      label hasVertexWeights = 0;
      label numericflag = 10*hasEdgeWeights+hasVertexWeights;
      str << baseval << ' ' << numericflag << nl;
      for (label cellI = 0; cellI < xadj.size()-1; cellI++)
      {
        label start = xadj[cellI];
        label end = xadj[cellI+1];
        str << end-start;
        for (label i = start; i < end; i++)
        {
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
  if (decompositionDict_.found("scotchCoeffs"))
  {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    string strategy;
    if (scotchCoeffs.readIfPresent("strategy", strategy))
    {
      if (debug)
      {
        Info<< "scotchDecomp : Using strategy " << strategy << endl;
      }
      SCOTCH_stratGraphMap(&stradat, strategy.c_str());
      //fprintf(stdout, "S\tStrat=");
      //SCOTCH_stratSave(&stradat, stdout);
      //fprintf(stdout, "\n");
    }
  }
  // Graph
  // ~~~~~
  List<label> velotab;
  // Check for externally provided cellweights and if so initialise weights
  // Note: min, not gMin since routine runs on master only.
  scalar minWeights = min(cWeights);
  if (!cWeights.empty())
  {
    if (minWeights <= 0)
    {
      WarningIn
      (
        "scotchDecomp::decompose(...)"
      )   << "Illegal minimum weight " << minWeights
        << endl;
    }
    if (cWeights.size() != xadj.size()-1)
    {
      FatalErrorIn
      (
        "scotchDecomp::decompose(...)"
      )   << "Number of cell weights " << cWeights.size()
        << " does not equal number of cells " << xadj.size()-1
        << exit(FatalError);
    }
    scalar velotabSum = sum(cWeights)/minWeights;
    scalar rangeScale(1.0);
    if (velotabSum > scalar(labelMax - 1))
    {
      // 0.9 factor of safety to avoid floating point round-off in
      // rangeScale tipping the subsequent sum over the integer limit.
      rangeScale = 0.9*scalar(labelMax - 1)/velotabSum;
      WarningIn
      (
        "scotchDecomp::decompose(...)"
      )   << "Sum of weights has overflowed integer: " << velotabSum
        << ", compressing weight scale by a factor of " << rangeScale
        << endl;
    }
    // Convert to integers.
    velotab.setSize(cWeights.size());
    forAll(velotab, i)
    {
      velotab[i] = int((cWeights[i]/minWeights - 1)*rangeScale) + 1;
    }
  }
  SCOTCH_Graph grafdat;
  check(SCOTCH_graphInit(&grafdat), "SCOTCH_graphInit");
  check
  (
    SCOTCH_graphBuild
    (
      &grafdat,
      0,                      // baseval, c-style numbering
      xadj.size()-1,          // vertnbr, nCells
      xadj.begin(),           // verttab, start index per cell into adjncy
      &xadj[1],               // vendtab, end index  ,,
      velotab.begin(),        // velotab, vertex weights
      NULL,                   // vlbltab
      adjncy.size(),          // edgenbr, number of arcs
      adjncy.begin(),         // edgetab
      NULL                    // edlotab, edge weights
    ),
    "SCOTCH_graphBuild"
  );
  check(SCOTCH_graphCheck(&grafdat), "SCOTCH_graphCheck");
  // Architecture
  // ~~~~~~~~~~~~
  // (fully connected network topology since using switch)
  SCOTCH_Arch archdat;
  check(SCOTCH_archInit(&archdat), "SCOTCH_archInit");
  List<label> processorWeights;
  if (decompositionDict_.found("scotchCoeffs"))
  {
    const dictionary& scotchCoeffs =
      decompositionDict_.subDict("scotchCoeffs");
    scotchCoeffs.readIfPresent("processorWeights", processorWeights);
  }
  if (processorWeights.size())
  {
    if (debug)
    {
      Info<< "scotchDecomp : Using procesor weights " << processorWeights
        << endl;
    }
    check
    (
      SCOTCH_archCmpltw(&archdat, nProcessors_, processorWeights.begin()),
      "SCOTCH_archCmpltw"
    );
  }
  else
  {
    check
    (
      SCOTCH_archCmplt(&archdat, nProcessors_),
      "SCOTCH_archCmplt"
    );
    //- Hack to test clustering. Note that finalDecomp is non-compact
    //  numbers!
    //
    ////- Set up variable sizes architecture
    //check
    //(
    //    SCOTCH_archVcmplt(&archdat),
    //    "SCOTCH_archVcmplt"
    //);
    //
    ////- Stategy flags: go for quality or load balance (or leave default)
    //SCOTCH_Num straval = 0;
    ////straval |= SCOTCH_STRATQUALITY;
    ////straval |= SCOTCH_STRATQUALITY;
    //
    ////- Number of cells per agglomeration
    ////SCOTCH_Num agglomSize = SCOTCH_archSize(&archdat);
    //SCOTCH_Num agglomSize = 3;
    //
    ////- Build strategy for agglomeration
    //check
    //(
    //    SCOTCH_stratGraphClusterBuild
    //    (
    //        &stradat,   // strategy to build
    //        straval,    // strategy flags
    //        agglomSize, // cells per cluster
    //        1.0,        // weight?
    //        0.01        // max load imbalance
    //    ),
    //    "SCOTCH_stratGraphClusterBuild"
    //);
  }
  //SCOTCH_Mapping mapdat;
  //SCOTCH_graphMapInit(&grafdat, &mapdat, &archdat, NULL);
  //SCOTCH_graphMapCompute(&grafdat, &mapdat, &stradat); /* Perform mapping */
  //SCOTCH_graphMapExit(&grafdat, &mapdat);
  // Hack:switch off fpu error trapping
#   ifdef FE_NOMASK_ENV
  int oldExcepts = fedisableexcept
  (
    FE_DIVBYZERO
   | FE_INVALID
   | FE_OVERFLOW
  );
#   endif
  finalDecomp.setSize(xadj.size()-1);
  finalDecomp = 0;
  check
  (
    SCOTCH_graphMap
    (
      &grafdat,
      &archdat,
      &stradat,           // const SCOTCH_Strat *
      finalDecomp.begin() // parttab
    ),
    "SCOTCH_graphMap"
  );
#   ifdef FE_NOMASK_ENV
  feenableexcept(oldExcepts);
#   endif
  //finalDecomp.setSize(xadj.size()-1);
  //check
  //(
  //    SCOTCH_graphPart
  //    (
  //        &grafdat,
  //        nProcessors_,       // partnbr
  //        &stradat,           // const SCOTCH_Strat *
  //        finalDecomp.begin() // parttab
  //    ),
  //    "SCOTCH_graphPart"
  //);
  // Release storage for graph
  SCOTCH_graphExit(&grafdat);
  // Release storage for strategy
  SCOTCH_stratExit(&stradat);
  // Release storage for network topology
  SCOTCH_archExit(&archdat);
  return 0;
}
// Constructors 
mousse::scotchDecomp::scotchDecomp(const dictionary& decompositionDict)
:
  decompositionMethod(decompositionDict)
{}
// Member Functions 
mousse::labelList mousse::scotchDecomp::decompose
(
  const polyMesh& mesh,
  const pointField& points,
  const scalarField& pointWeights
)
{
  if (points.size() != mesh.nCells())
  {
    FatalErrorIn
    (
      "scotchDecomp::decompose(const polyMesh&, const pointField&"
      ", const scalarField&)"
    )   << "Can use this decomposition method only for the whole mesh"
      << endl
      << "and supply one coordinate (cellCentre) for every cell." << endl
      << "The number of coordinates " << points.size() << endl
      << "The number of cells in the mesh " << mesh.nCells()
      << exit(FatalError);
  }
  // Calculate local or global (if Pstream::parRun()) connectivity
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
  labelList decomp(finalDecomp.size());
  forAll(decomp, i)
  {
    decomp[i] = finalDecomp[i];
  }
  return decomp;
}
mousse::labelList mousse::scotchDecomp::decompose
(
  const polyMesh& mesh,
  const labelList& agglom,
  const pointField& agglomPoints,
  const scalarField& pointWeights
)
{
  if (agglom.size() != mesh.nCells())
  {
    FatalErrorIn
    (
      "scotchDecomp::decompose"
      "(const polyMesh&, const labelList&, const pointField&"
      ", const scalarField&)"
    )   << "Size of cell-to-coarse map " << agglom.size()
      << " differs from number of cells in mesh " << mesh.nCells()
      << exit(FatalError);
  }
  // Calculate local or global (if Pstream::parRun()) connectivity
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
  // Rework back into decomposition for original mesh_
  labelList fineDistribution(agglom.size());
  forAll(fineDistribution, i)
  {
    fineDistribution[i] = finalDecomp[agglom[i]];
  }
  return fineDistribution;
}
mousse::labelList mousse::scotchDecomp::decompose
(
  const labelListList& globalCellCells,
  const pointField& cellCentres,
  const scalarField& cWeights
)
{
  if (cellCentres.size() != globalCellCells.size())
  {
    FatalErrorIn
    (
      "scotchDecomp::decompose"
      "(const labelListList&, const pointField&, const scalarField&)"
    )   << "Inconsistent number of cells (" << globalCellCells.size()
      << ") and number of cell centres (" << cellCentres.size()
      << ")." << exit(FatalError);
  }
  // Make Metis CSR (Compressed Storage Format) storage
  //   adjncy      : contains neighbours (= edges in graph)
  //   xadj(celli) : start of information in adjncy for celli
  CompactListList<label> cellCells(globalCellCells);
  // Decompose using weights
  List<label> finalDecomp;
  decompose
  (
    "scotch",
    cellCells.m(),
    cellCells.offsets(),
    cWeights,
    finalDecomp
  );
  // Copy back to labelList
  labelList decomp(finalDecomp.size());
  forAll(decomp, i)
  {
    decomp[i] = finalDecomp[i];
  }
  return decomp;
}
