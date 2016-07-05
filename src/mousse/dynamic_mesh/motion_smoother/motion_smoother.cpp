// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "motion_smoother.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(motionSmoother, 0);

}


// Constructors 
mousse::motionSmoother::motionSmoother
(
  polyMesh& mesh,
  pointMesh& pMesh,
  indirectPrimitivePatch& pp,
  const labelList& adaptPatchIDs,
  const dictionary& paramDict
)
:
  motionSmootherData{pMesh},
  motionSmootherAlgo
  {
    mesh,
    pMesh,
    pp,
    motionSmootherData::displacement_,
    motionSmootherData::scale_,
    motionSmootherData::oldPoints_,
    adaptPatchIDs,
    paramDict
  }
{}


mousse::motionSmoother::motionSmoother
(
  polyMesh& mesh,
  indirectPrimitivePatch& pp,
  const labelList& adaptPatchIDs,
  const pointVectorField& displacement,
  const dictionary& paramDict
)
:
  motionSmootherData{displacement},
  motionSmootherAlgo
  {
    mesh,
    const_cast<pointMesh&>(displacement.mesh()),
    pp,
    motionSmootherData::displacement_,
    motionSmootherData::scale_,
    motionSmootherData::oldPoints_,
    adaptPatchIDs,
    paramDict
  }
{}

