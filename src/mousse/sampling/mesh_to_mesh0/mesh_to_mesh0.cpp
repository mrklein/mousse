// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "mesh_to_mesh0.hpp"
#include "processor_fv_patch.hpp"
#include "demand_driven_data.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(meshToMesh0, 0);

}

const mousse::scalar mousse::meshToMesh0::directHitTol = 1e-5;


// Constructors 
mousse::meshToMesh0::meshToMesh0
(
  const fvMesh& meshFrom,
  const fvMesh& meshTo,
  const HashTable<word>& patchMap,
  const wordList& cuttingPatchNames
)
:
  fromMesh_{meshFrom},
  toMesh_{meshTo},
  patchMap_{patchMap},
  cellAddressing_{toMesh_.nCells()},
  boundaryAddressing_{toMesh_.boundaryMesh().size()},
  inverseDistanceWeightsPtr_{NULL},
  inverseVolumeWeightsPtr_{NULL},
  cellToCellAddressingPtr_{NULL},
  V_{0.0}
{
  FOR_ALL(fromMesh_.boundaryMesh(), patchi) {
    fromMeshPatches_.insert
    (
      fromMesh_.boundaryMesh()[patchi].name(),
      patchi
    );
  }
  FOR_ALL(toMesh_.boundaryMesh(), patchi) {
    toMeshPatches_.insert
    (
      toMesh_.boundaryMesh()[patchi].name(),
      patchi
    );
  }
  FOR_ALL(cuttingPatchNames, i) {
    if (toMeshPatches_.found(cuttingPatchNames[i])) {
      cuttingPatches_.insert
      (
        cuttingPatchNames[i],
        toMeshPatches_.find(cuttingPatchNames[i])()
      );
    } else {
      WARNING_IN
      (
        "meshToMesh0::meshToMesh0"
        "(const fvMesh& meshFrom, const fvMesh& meshTo,"
        "const HashTable<word>& patchMap,"
        "const wordList& cuttingPatchNames)"
      )
      << "Cannot find cutting-patch " << cuttingPatchNames[i]
      << " in destination mesh" << endl;
    }
  }
  FOR_ALL(toMesh_.boundaryMesh(), patchi) {
    // Add the processor patches in the toMesh to the cuttingPatches list
    if (isA<processorPolyPatch>(toMesh_.boundaryMesh()[patchi])) {
      cuttingPatches_.insert
      (
        toMesh_.boundaryMesh()[patchi].name(),
        patchi
      );
    }
  }
  calcAddressing();
}


mousse::meshToMesh0::meshToMesh0
(
  const fvMesh& meshFrom,
  const fvMesh& meshTo
)
:
  fromMesh_{meshFrom},
  toMesh_{meshTo},
  cellAddressing_{toMesh_.nCells()},
  boundaryAddressing_{toMesh_.boundaryMesh().size()},
  inverseDistanceWeightsPtr_{NULL},
  inverseVolumeWeightsPtr_{NULL},
  cellToCellAddressingPtr_{NULL},
  V_{0.0}
{
  // check whether both meshes have got the same number
  // of boundary patches
  if (fromMesh_.boundary().size() != toMesh_.boundary().size()) {
    FATAL_ERROR_IN
    (
      "meshToMesh0::meshToMesh0"
      "(const fvMesh& meshFrom, const fvMesh& meshTo)"
    )
    << "Incompatible meshes: different number of patches, "
    << "fromMesh = " << fromMesh_.boundary().size()
    << ", toMesh = " << toMesh_.boundary().size()
    << exit(FatalError);
  }
  FOR_ALL(fromMesh_.boundaryMesh(), patchi) {
    if (fromMesh_.boundaryMesh()[patchi].name()
        != toMesh_.boundaryMesh()[patchi].name()) {
      FATAL_ERROR_IN
      (
        "meshToMesh0::meshToMesh0"
        "(const fvMesh& meshFrom, const fvMesh& meshTo)"
      )
      << "Incompatible meshes: different patch names for patch "
      << patchi
      << ", fromMesh = " << fromMesh_.boundary()[patchi].name()
      << ", toMesh = " << toMesh_.boundary()[patchi].name()
      << exit(FatalError);
    }
    if (fromMesh_.boundaryMesh()[patchi].type()
        != toMesh_.boundaryMesh()[patchi].type()) {
      FATAL_ERROR_IN
      (
        "meshToMesh0::meshToMesh0"
        "(const fvMesh& meshFrom, const fvMesh& meshTo)"
      )
      << "Incompatible meshes: different patch types for patch "
      << patchi
      << ", fromMesh = " << fromMesh_.boundary()[patchi].type()
      << ", toMesh = " << toMesh_.boundary()[patchi].type()
      << exit(FatalError);
    }
    fromMeshPatches_.insert
    (
      fromMesh_.boundaryMesh()[patchi].name(),
      patchi
    );
    toMeshPatches_.insert
    (
      toMesh_.boundaryMesh()[patchi].name(),
      patchi
    );
    patchMap_.insert
    (
      toMesh_.boundaryMesh()[patchi].name(),
      fromMesh_.boundaryMesh()[patchi].name()
    );
  }
  calcAddressing();
}


// Destructor 
mousse::meshToMesh0::~meshToMesh0()
{
  deleteDemandDrivenData(inverseDistanceWeightsPtr_);
  deleteDemandDrivenData(inverseVolumeWeightsPtr_);
  deleteDemandDrivenData(cellToCellAddressingPtr_);
}

