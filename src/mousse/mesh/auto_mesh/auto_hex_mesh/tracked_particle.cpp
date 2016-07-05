// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "tracked_particle.hpp"


// Constructors 
mousse::trackedParticle::trackedParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const point& end,
  const label level,
  const label i,
  const label j,
  const label k
)
:
  particle{mesh, position, cellI, tetFaceI, tetPtI},
  end_{end},
  level_{level},
  i_{i},
  j_{j},
  k_{k}
{}


mousse::trackedParticle::trackedParticle
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  particle{mesh, is, readFields}
{
  if (readFields) {
    if (is.format() == IOstream::ASCII) {
      is >> end_;
      level_ = readLabel(is);
      i_ = readLabel(is);
      j_ = readLabel(is);
      k_ = readLabel(is);
    } else {
      is.read
      (
        reinterpret_cast<char*>(&end_),
        sizeof(end_) + sizeof(level_) + sizeof(i_) + sizeof(j_) + sizeof(k_)
      );
    }
  }
  // Check state of Istream
  is.check
  (
    "trackedParticle::trackedParticle"
    "(const Cloud<trackedParticle>&, Istream&, bool)"
  );
}


// Member Functions 
bool mousse::trackedParticle::move
(
  trackingData& td,
  const scalar trackTime
)
{
  td.switchProcessor = false;
  scalar tEnd = (1.0 - stepFraction())*trackTime;
  scalar dtMax = tEnd;
  if (tEnd <= SMALL && onBoundary()) {
    // This is a hack to handle particles reaching their endpoint
    // on a processor boundary. If the endpoint is on a processor face
    // it currently gets transferred backwards and forwards infinitely.
    // Remove the particle
    td.keepParticle = false;
  } else {
    td.keepParticle = true;
    while (td.keepParticle && !td.switchProcessor && tEnd > SMALL) {
      // set the lagrangian time-step
      scalar dt = min(dtMax, tEnd);
      // mark visited cell with max level.
      td.maxLevel_[cell()] = max(td.maxLevel_[cell()], level_);
      dt *= trackToFace(end_, td);
      tEnd -= dt;
      stepFraction() = 1.0 - tEnd/trackTime;
    }
  }
  return td.keepParticle;
}


bool mousse::trackedParticle::hitPatch
(
  const polyPatch&,
  trackingData&,
  const label /*patchI*/,
  const scalar /*trackFraction*/,
  const tetIndices& /*tetIs*/
)
{
  return false;
}


void mousse::trackedParticle::hitWedgePatch
(
  const wedgePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::hitSymmetryPlanePatch
(
  const symmetryPlanePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::hitSymmetryPatch
(
  const symmetryPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::hitCyclicPatch
(
  const cyclicPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::hitProcessorPatch
(
  const processorPolyPatch&,
  trackingData& td
)
{
  // Move to different processor
  td.switchProcessor = true;
}


void mousse::trackedParticle::hitWallPatch
(
  const wallPolyPatch&,
  trackingData& td,
  const tetIndices&
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::hitPatch
(
  const polyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::trackedParticle::correctAfterParallelTransfer
(
  const label patchI,
  trackingData& td
)
{
  particle::correctAfterParallelTransfer(patchI, td);
  label edgeI = k();
  if (edgeI == -1)
    return;
  label featI = i();
  // Mark edge we're currently on (was set on sending processor but not
  // receiving sender)
  td.featureEdgeVisited_[featI].set(edgeI, 1u);
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const trackedParticle& p)
{
  if (os.format() == IOstream::ASCII) {
    os << static_cast<const particle&>(p)
      << token::SPACE << p.end_
      << token::SPACE << p.level_
      << token::SPACE << p.i_
      << token::SPACE << p.j_
      << token::SPACE << p.k_;
  } else {
    os << static_cast<const particle&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.end_),
      sizeof(p.end_) + sizeof(p.level_)
      + sizeof(p.i_) + sizeof(p.j_) + sizeof(p.k_)
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const trackedParticle&)");
  return os;
}

