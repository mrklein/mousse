// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "manual_gamg_proc_agglomeration.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "gamg_agglomeration.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(manualGAMGProcAgglomeration, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGProcAgglomeration,
  manualGAMGProcAgglomeration,
  GAMGAgglomeration
);

}

// Private Member Functions
// Constructors
mousse::manualGAMGProcAgglomeration::manualGAMGProcAgglomeration
(
  GAMGAgglomeration& agglom,
  const dictionary& controlDict
)
:
  GAMGProcAgglomeration(agglom, controlDict),
  procAgglomMaps_(controlDict.lookup("processorAgglomeration"))
{}

// Destructor
mousse::manualGAMGProcAgglomeration::
~manualGAMGProcAgglomeration()
{
  FOR_ALL_REVERSE(comms_, i)
  {
    if (comms_[i] != -1)
    {
      UPstream::freeCommunicator(comms_[i]);
    }
  }
}

// Member Functions
bool mousse::manualGAMGProcAgglomeration::agglomerate()
{
  if (debug)
  {
    Pout<< nl << "Starting mesh overview" << endl;
    printStats(Pout, agglom_);
  }
  if (agglom_.size() >= 1)
  {
    FOR_ALL(procAgglomMaps_, i)
    {
      const label fineLevelIndex = procAgglomMaps_[i].first();
      if (fineLevelIndex >= agglom_.size())
      {
        WARNING_IN("manualGAMGProcAgglomeration::agglomerate()")
          << "Ignoring specification for level " << fineLevelIndex
          << " since outside agglomeration." << endl;
        continue;
      }
      if (agglom_.hasMeshLevel(fineLevelIndex))
      {
        // Get the fine mesh
        const lduMesh& levelMesh = agglom_.meshLevel(fineLevelIndex);
        label nProcs = UPstream::nProcs(levelMesh.comm());
        if (nProcs > 1)
        {
          // My processor id
          const label myProcID = Pstream::myProcNo(levelMesh.comm());
          const List<labelList>& clusters =
            procAgglomMaps_[i].second();
          // Coarse to fine master processor
          labelList coarseToMaster(clusters.size());
          // Fine to coarse map
          labelList procAgglomMap(nProcs, -1);
          // Cluster for my processor (with master index first)
          labelList agglomProcIDs;
          FOR_ALL(clusters, coarseI)
          {
            const labelList& cluster = clusters[coarseI];
            coarseToMaster[coarseI] = cluster[0];
            FOR_ALL(cluster, i)
            {
              procAgglomMap[cluster[i]] = coarseI;
            }
            label masterIndex = findIndex
            (
              cluster,
              coarseToMaster[coarseI]
            );
            if (masterIndex == -1)
            {
              FATAL_ERROR_IN
              (
                "manualGAMGProcAgglomeration::agglomerate()"
              )
              << "At level " << fineLevelIndex
              << " the master processor "
              << coarseToMaster[coarseI]
              << " is not in the cluster "
              << cluster
              << exit(FatalError);
            }
            if (findIndex(cluster, myProcID) != -1)
            {
              // This is my cluster. Make sure master index is
              // first
              agglomProcIDs = cluster;
              Swap(agglomProcIDs[0], agglomProcIDs[masterIndex]);
            }
          }
          // Check that we've done all processors
          if (findIndex(procAgglomMap, -1) != -1)
          {
            FATAL_ERROR_IN
            (
              "manualGAMGProcAgglomeration::agglomerate()"
            )
            << "At level " << fineLevelIndex
            << " processor "
            << findIndex(procAgglomMap, -1)
            << " is not in any cluster"
            << exit(FatalError);
          }
          // Allocate a communicator for the processor-agglomerated
          // matrix
          comms_.append
          (
            UPstream::allocateCommunicator
            (
              levelMesh.comm(),
              coarseToMaster
            )
          );
          // Use procesor agglomeration maps to do the actual
          // collecting
          if (Pstream::myProcNo(levelMesh.comm()) != -1)
          {
            GAMGProcAgglomeration::agglomerate
            (
              fineLevelIndex,
              procAgglomMap,
              coarseToMaster,
              agglomProcIDs,
              comms_.last()
            );
          }
        }
      }
    }
    // Print a bit
    if (debug)
    {
      Pout<< nl << "Agglomerated mesh overview" << endl;
      printStats(Pout, agglom_);
    }
  }
  return true;
}
