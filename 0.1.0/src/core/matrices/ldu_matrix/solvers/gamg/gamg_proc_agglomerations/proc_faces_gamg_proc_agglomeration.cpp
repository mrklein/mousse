// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "proc_faces_gamg_proc_agglomeration.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "gamg_agglomeration.hpp"
#include "random.hpp"
#include "ldu_mesh.hpp"
#include "processor_ldu_interface.hpp"
#include "processor_gamg_interface.hpp"
#include "pair_gamg_agglomeration.hpp"
#include "map.hpp"
#include "label.hpp"
#include "uindirect_list.hpp"

// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(procFacesGAMGProcAgglomeration, 0);

ADD_TO_RUN_TIME_SELECTION_TABLE
(
  GAMGProcAgglomeration,
  procFacesGAMGProcAgglomeration,
  GAMGAgglomeration
);

}

// Private Member Functions
// Create single cell mesh
mousse::autoPtr<mousse::lduPrimitiveMesh>
mousse::procFacesGAMGProcAgglomeration::singleCellMesh
(
  const label singleCellMeshComm,
  const lduMesh& mesh,
  scalarField& faceWeights
) const
{
  // Count number of faces per processor
  List<Map<label> > procFaces(UPstream::nProcs(mesh.comm()));
  Map<label>& myNeighbours = procFaces[UPstream::myProcNo(mesh.comm())];
  {
    const lduInterfacePtrsList interfaces(mesh.interfaces());
    FOR_ALL(interfaces, intI)
    {
      if (interfaces.set(intI))
      {
        const processorLduInterface& pp =
          refCast<const processorLduInterface>
          (
            interfaces[intI]
          );
        label size = interfaces[intI].faceCells().size();
        myNeighbours.insert(pp.neighbProcNo(), size);
      }
    }
  }
  Pstream::gatherList(procFaces, Pstream::msgType(), mesh.comm());
  Pstream::scatterList(procFaces, Pstream::msgType(), mesh.comm());
  autoPtr<lduPrimitiveMesh> singleCellMeshPtr;
  if (Pstream::master(mesh.comm()))
  {
    // I am master
    label nCells = Pstream::nProcs(mesh.comm());
    DynamicList<label> l(3*nCells);
    DynamicList<label> u(3*nCells);
    DynamicList<scalar> weight(3*nCells);
    DynamicList<label> nbrs;
    DynamicList<scalar> weights;
    FOR_ALL(procFaces, procI)
    {
      const Map<label>& neighbours = procFaces[procI];
      // Add all the higher processors
      nbrs.clear();
      weights.clear();
      FOR_ALL_CONST_ITER(Map<label>, neighbours, iter)
      {
        if (iter.key() > procI)
        {
          nbrs.append(iter.key());
          weights.append(iter());
        }
        sort(nbrs);
        FOR_ALL(nbrs, i)
        {
          l.append(procI);
          u.append(nbrs[i]);
          weight.append(weights[i]);
        }
      }
    }
    faceWeights.transfer(weight);
    PtrList<const lduInterface> primitiveInterfaces(0);
    const lduSchedule ps(0);
    singleCellMeshPtr.reset
    (
      new lduPrimitiveMesh
      (
        nCells,
        l,
        u,
        primitiveInterfaces,
        ps,
        singleCellMeshComm
      )
    );
  }
  return singleCellMeshPtr;
}
mousse::tmp<mousse::labelField>
mousse::procFacesGAMGProcAgglomeration::processorAgglomeration
(
  const lduMesh& mesh
) const
{
  label singleCellMeshComm = UPstream::allocateCommunicator
  (
    mesh.comm(),
    labelList(1, label(0))            // only processor 0
  );
  scalarField faceWeights;
  autoPtr<lduPrimitiveMesh> singleCellMeshPtr
  (
    singleCellMesh
    (
      singleCellMeshComm,
      mesh,
      faceWeights
    )
  );
  tmp<labelField> tfineToCoarse(new labelField(0));
  labelField& fineToCoarse = tfineToCoarse();
  if (singleCellMeshPtr.valid())
  {
    // On master call the agglomerator
    const lduPrimitiveMesh& singleCellMesh = singleCellMeshPtr();
    label nCoarseProcs;
    fineToCoarse = pairGAMGAgglomeration::agglomerate
    (
      nCoarseProcs,
      singleCellMesh,
      faceWeights
    );
    labelList coarseToMaster(nCoarseProcs, labelMax);
    FOR_ALL(fineToCoarse, cellI)
    {
      label coarseI = fineToCoarse[cellI];
      coarseToMaster[coarseI] = min(coarseToMaster[coarseI], cellI);
    }
    // Sort according to master and redo restriction
    labelList newToOld;
    sortedOrder(coarseToMaster, newToOld);
    labelList oldToNew(invert(newToOld.size(), newToOld));
    fineToCoarse = UIndirectList<label>(oldToNew, fineToCoarse)();
  }
  Pstream::scatter(fineToCoarse, Pstream::msgType(), mesh.comm());
  UPstream::freeCommunicator(singleCellMeshComm);
  return tfineToCoarse;
}
bool mousse::procFacesGAMGProcAgglomeration::doProcessorAgglomeration
(
  const lduMesh& mesh
) const
{
  // Check the need for further agglomeration on all processors
  bool doAgg = mesh.lduAddr().size() < nAgglomeratingCells_;
  mesh.reduce(doAgg, orOp<bool>());
  return doAgg;
}
// Constructors
mousse::procFacesGAMGProcAgglomeration::procFacesGAMGProcAgglomeration
(
  GAMGAgglomeration& agglom,
  const dictionary& controlDict
)
:
  GAMGProcAgglomeration(agglom, controlDict),
  nAgglomeratingCells_(readLabel(controlDict.lookup("nAgglomeratingCells")))
{}

// Destructor
mousse::procFacesGAMGProcAgglomeration::~procFacesGAMGProcAgglomeration()
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
bool mousse::procFacesGAMGProcAgglomeration::agglomerate()
{
  if (debug)
  {
    Pout<< nl << "Starting mesh overview" << endl;
    printStats(Pout, agglom_);
  }
  if (agglom_.size() >= 1)
  {
    Random rndGen(0);
    for
    (
      label fineLevelIndex = 2;
      fineLevelIndex < agglom_.size();
      fineLevelIndex++
    )
    {
      if (agglom_.hasMeshLevel(fineLevelIndex))
      {
        // Get the fine mesh
        const lduMesh& levelMesh = agglom_.meshLevel(fineLevelIndex);
        label levelComm = levelMesh.comm();
        label nProcs = UPstream::nProcs(levelComm);
        if (nProcs > 1 && doProcessorAgglomeration(levelMesh))
        {
          tmp<labelField> tprocAgglomMap
          (
            processorAgglomeration(levelMesh)
          );
          const labelField& procAgglomMap = tprocAgglomMap();
          // Master processor
          labelList masterProcs;
          // Local processors that agglomerate. agglomProcIDs[0] is in
          // masterProc.
          List<label> agglomProcIDs;
          GAMGAgglomeration::calculateRegionMaster
          (
            levelComm,
            procAgglomMap,
            masterProcs,
            agglomProcIDs
          );
          // Allocate a communicator for the processor-agglomerated
          // matrix
          comms_.append
          (
            UPstream::allocateCommunicator
            (
              levelComm,
              masterProcs
            )
          );
          // Use procesor agglomeration maps to do the actual
          // collecting.
          GAMGProcAgglomeration::agglomerate
          (
            fineLevelIndex,
            procAgglomMap,
            masterProcs,
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
  return true;
}
