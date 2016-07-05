// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "find_cell_particle.hpp"


// Constructors 
mousse::findCellParticle::findCellParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const point& end,
  const label data
)
:
  particle{mesh, position, cellI, tetFaceI, tetPtI},
  end_{end},
  data_{data}
{}


mousse::findCellParticle::findCellParticle
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
      data_ = readLabel(is);
    } else {
      is.read(reinterpret_cast<char*>(&end_), sizeof(end_) + sizeof(data_));
    }
  }
  // Check state of Istream
  is.check
  (
    "findCellParticle::findCellParticle"
    "(const Cloud<findCellParticle>&, Istream&, bool)"
  );
}


// Member Functions 
bool mousse::findCellParticle::move
(
  trackingData& td,
  const scalar maxTrackLen
)
{
  td.switchProcessor = false;
  td.keepParticle = true;
  scalar tEnd = (1.0 - stepFraction())*maxTrackLen;
  scalar dtMax = tEnd;
  while (td.keepParticle && !td.switchProcessor && tEnd > SMALL) {
    // set the lagrangian time-step
    scalar dt = min(dtMax, tEnd);
    dt *= trackToFace(end_, td);
    tEnd -= dt;
    stepFraction() = 1.0 - tEnd/maxTrackLen;
  }
  if (tEnd < SMALL || !td.keepParticle) {
    // Hit endpoint or patch. If patch hit could do fancy stuff but just
    // to use the patch point is good enough for now.
    td.cellToData()[cell()].append(data());
    td.cellToEnd()[cell()].append(position());
  }
  return td.keepParticle;
}


bool mousse::findCellParticle::hitPatch
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


void mousse::findCellParticle::hitWedgePatch
(
  const wedgePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::findCellParticle::hitSymmetryPlanePatch
(
  const symmetryPlanePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::findCellParticle::hitSymmetryPatch
(
  const symmetryPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::findCellParticle::hitCyclicPatch
(
  const cyclicPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::findCellParticle::hitProcessorPatch
(
  const processorPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.switchProcessor = true;
}


void mousse::findCellParticle::hitWallPatch
(
  const wallPolyPatch&,
  trackingData& td,
  const tetIndices&
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::findCellParticle::hitPatch
(
  const polyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const findCellParticle& p)
{
  if (os.format() == IOstream::ASCII) {
    os << static_cast<const particle&>(p)
      << token::SPACE << p.end_
      << token::SPACE << p.data_;
  } else {
    os << static_cast<const particle&>(p);
    os.write
    (
      reinterpret_cast<const char*>(&p.end_),
      sizeof(p.end_) + sizeof(p.data_)
    );
  }
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const findCellParticle&)");
  return os;
}

