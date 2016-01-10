// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "reconstruct_lagrangian.hpp"
#include "label_io_list.hpp"
#include "passive_particle_cloud.hpp"
// Global Functions 
void mousse::reconstructLagrangianPositions
(
  const polyMesh& mesh,
  const word& cloudName,
  const PtrList<fvMesh>& meshes,
  const PtrList<labelIOList>& /*faceProcAddressing*/,
  const PtrList<labelIOList>& cellProcAddressing
)
{
  passiveParticleCloud lagrangianPositions
  (
    mesh,
    cloudName,
    IDLList<passiveParticle>()
  );
  FOR_ALL(meshes, i)
  {
    const labelList& cellMap = cellProcAddressing[i];
    // faceProcAddressing not required currently.
    // const labelList& faceMap = faceProcAddressing[i];
    Cloud<passiveParticle> lpi(meshes[i], cloudName, false);
    FOR_ALL_CONST_ITER(Cloud<passiveParticle>, lpi, iter)
    {
      const passiveParticle& ppi = iter();
      // // Inverting sign if necessary and subtracting 1 from
      // // faceProcAddressing
      // label mappedTetFace = mag(faceMap[ppi.tetFace()]) - 1;
      lagrangianPositions.append
      (
        new passiveParticle
        (
          mesh,
          ppi.position(),
          cellMap[ppi.cell()],
          false
        )
      );
    }
  }
  IOPosition<Cloud<passiveParticle> >(lagrangianPositions).write();
}
