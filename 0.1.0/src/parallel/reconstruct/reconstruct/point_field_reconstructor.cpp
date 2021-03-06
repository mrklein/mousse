// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "point_field_reconstructor.hpp"


// Constructors 
mousse::pointFieldReconstructor::pointFieldReconstructor
(
  const pointMesh& mesh,
  const PtrList<pointMesh>& procMeshes,
  const PtrList<labelIOList>& pointProcAddressing,
  const PtrList<labelIOList>& boundaryProcAddressing
)
:
  mesh_{mesh},
  procMeshes_{procMeshes},
  pointProcAddressing_{pointProcAddressing},
  boundaryProcAddressing_{boundaryProcAddressing},
  patchPointAddressing_{procMeshes.size()},
  nReconstructed_{0}
{
  // Inverse-addressing of the patch point labels.
  labelList pointMap{mesh_.size(), -1};
  // Create the pointPatch addressing
  FOR_ALL(procMeshes_, proci) {
    const pointMesh& procMesh = procMeshes_[proci];
    patchPointAddressing_[proci].setSize(procMesh.boundary().size());
    FOR_ALL(procMesh.boundary(), patchi) {
      if (boundaryProcAddressing_[proci][patchi] >= 0) {
        labelList& procPatchAddr = patchPointAddressing_[proci][patchi];
        procPatchAddr.setSize(procMesh.boundary()[patchi].size(), -1);
        const labelList& patchPointLabels =
          mesh_
            .boundary()[boundaryProcAddressing_[proci][patchi]]
            .meshPoints();
        // Create the inverse-addressing of the patch point labels.
        FOR_ALL(patchPointLabels, pointi) {
          pointMap[patchPointLabels[pointi]] = pointi;
        }
        const labelList& procPatchPoints =
          procMesh.boundary()[patchi].meshPoints();
        FOR_ALL(procPatchPoints, pointi) {
          procPatchAddr[pointi] =
            pointMap[pointProcAddressing_[proci][procPatchPoints[pointi]]];
        }
        if (procPatchAddr.size() && min(procPatchAddr) < 0) {
          FATAL_ERROR_IN
          (
            "pointFieldReconstructor::pointFieldReconstructor"
            "(\n"
            "    const pointMesh& mesh,\n"
            "    const PtrList<pointMesh>& procMeshes,\n"
            "    const PtrList<labelIOList>& pointProcAddressing,\n"
            "    const PtrList<labelIOList>& "
            "boundaryProcAddressing\n"
            ")"
          )
          << "Incomplete patch point addressing"
          << abort(FatalError);
        }
      }
    }
  }
}

