// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "lagrangian_field_decomposer.hpp"
// Constructors 
// Construct from components
mousse::lagrangianFieldDecomposer::lagrangianFieldDecomposer
(
  const polyMesh& mesh,
  const polyMesh& procMesh,
  const labelList& faceProcAddressing,
  const labelList& cellProcAddressing,
  const word& cloudName,
  const Cloud<indexedParticle>& lagrangianPositions,
  const List<SLList<indexedParticle*>*>& cellParticles
)
:
  procMesh_(procMesh),
  positions_(procMesh, cloudName, false),
  particleIndices_(lagrangianPositions.size())
{
  label pi = 0;
  // faceProcAddressing not required currently
  // labelList decodedProcFaceAddressing(faceProcAddressing.size());
  // forAll(faceProcAddressing, i)
  // {
  //     decodedProcFaceAddressing[i] = mag(faceProcAddressing[i]) - 1;
  // }
  forAll(cellProcAddressing, procCelli)
  {
    label celli = cellProcAddressing[procCelli];
    if (cellParticles[celli])
    {
      SLList<indexedParticle*>& particlePtrs = *cellParticles[celli];
      forAllConstIter(SLList<indexedParticle*>, particlePtrs, iter)
      {
        const indexedParticle& ppi = *iter();
        particleIndices_[pi++] = ppi.index();
        // label mappedTetFace = findIndex
        // (
        //     decodedProcFaceAddressing,
        //     ppi.tetFace()
        // );
        // if (mappedTetFace == -1)
        // {
        //     FatalErrorIn
        //     (
        //         "mousse::lagrangianFieldDecomposer"
        //         "::lagrangianFieldDecomposer"
        //         "("
        //             "const polyMesh& mesh, "
        //             "const polyMesh& procMesh, "
        //             "const labelList& faceProcAddressing, "
        //             "const labelList& cellProcAddressing, "
        //             "const word& cloudName, "
        //             "const Cloud<indexedParticle>& "
        //             "lagrangianPositions, "
        //             "const List<SLList<indexedParticle*>*>& "
        //             "cellParticles"
        //         ")"
        //     )   << "Face lookup failure." << nl
        //         << abort(FatalError);
        // }
        positions_.append
        (
          new passiveParticle
          (
            procMesh,
            ppi.position(),
            procCelli,
            false
          )
        );
      }
    }
  }
  particleIndices_.setSize(pi);
  IOPosition<Cloud<passiveParticle> >(positions_).write();
}
