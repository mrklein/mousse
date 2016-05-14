// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lagrangian_field_decomposer.hpp"


// Constructors 
// Construct from components
mousse::lagrangianFieldDecomposer::lagrangianFieldDecomposer
(
  const polyMesh& /*mesh*/,
  const polyMesh& procMesh,
  const labelList& /*faceProcAddressing*/,
  const labelList& cellProcAddressing,
  const word& cloudName,
  const Cloud<indexedParticle>& lagrangianPositions,
  const List<SLList<indexedParticle*>*>& cellParticles
)
:
  procMesh_{procMesh},
  positions_{procMesh, cloudName, false},
  particleIndices_{lagrangianPositions.size()}
{
  label pi = 0;
  FOR_ALL(cellProcAddressing, procCelli) {
    label celli = cellProcAddressing[procCelli];
    if (!cellParticles[celli])
      continue;
    SLList<indexedParticle*>& particlePtrs = *cellParticles[celli];
    FOR_ALL_CONST_ITER(SLList<indexedParticle*>, particlePtrs, iter) {
      const indexedParticle& ppi = *iter();
      particleIndices_[pi++] = ppi.index();
      positions_.append
      (
        new passiveParticle
        {
          procMesh,
          ppi.position(),
          procCelli,
          false
        }
      );
    }
  }
  particleIndices_.setSize(pi);
  IOPosition<Cloud<passiveParticle>>(positions_).write();
}

