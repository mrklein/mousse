// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "particle.hpp"
#include "transform.hpp"


// Static Data Members
mousse::label mousse::particle::particleCount_ = 0;
const mousse::scalar mousse::particle::trackingCorrectionTol = 1e-5;
const mousse::scalar mousse::particle::lambdaDistanceToleranceCoeff = 1e3*SMALL;
const mousse::scalar mousse::particle::minStepFractionTol = 1e5*SMALL;

namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(particle, 0);

}


// Constructors 
mousse::particle::particle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI
)
:
  mesh_{mesh},
  position_{position},
  cellI_{cellI},
  faceI_{-1},
  stepFraction_{0.0},
  tetFaceI_{tetFaceI},
  tetPtI_{tetPtI},
  origProc_{Pstream::myProcNo()},
  origId_{getNewParticleID()}
{}


mousse::particle::particle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  bool doCellFacePt
)
:
  mesh_{mesh},
  position_{position},
  cellI_{cellI},
  faceI_{-1},
  stepFraction_{0.0},
  tetFaceI_{-1},
  tetPtI_{-1},
  origProc_{Pstream::myProcNo()},
  origId_{getNewParticleID()}
{
  if (doCellFacePt) {
    initCellFacePt();
  }
}



mousse::particle::particle(const particle& p)
:
  mesh_{p.mesh_},
  position_{p.position_},
  cellI_{p.cellI_},
  faceI_{p.faceI_},
  stepFraction_{p.stepFraction_},
  tetFaceI_{p.tetFaceI_},
  tetPtI_{p.tetPtI_},
  origProc_{p.origProc_},
  origId_{p.origId_}
{}


mousse::particle::particle(const particle& p, const polyMesh& mesh)
:
  mesh_{mesh},
  position_{p.position_},
  cellI_{p.cellI_},
  faceI_{p.faceI_},
  stepFraction_{p.stepFraction_},
  tetFaceI_{p.tetFaceI_},
  tetPtI_{p.tetPtI_},
  origProc_{p.origProc_},
  origId_{p.origId_}
{}


// Member Functions 
void mousse::particle::transformProperties(const tensor&)
{}


void mousse::particle::transformProperties(const vector&)
{}


mousse::scalar mousse::particle::wallImpactDistance(const vector&) const
{
  Info << "particle::wallImpactDistance" << endl;
  return 0.0;
}


// Friend Operators
bool mousse::operator==(const particle& pA, const particle& pB)
{
  return (pA.origProc() == pB.origProc() && pA.origId() == pB.origId());
}


bool mousse::operator!=(const particle& pA, const particle& pB)
{
  return !(pA == pB);
}

