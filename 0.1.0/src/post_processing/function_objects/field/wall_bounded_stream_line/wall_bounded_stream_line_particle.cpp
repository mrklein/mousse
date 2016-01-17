// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "wall_bounded_stream_line_particle.hpp"
#include "vector_field_io_field.hpp"
// Private Member Functions 
mousse::vector mousse::wallBoundedStreamLineParticle::interpolateFields
(
  const trackingData& td,
  const point& position,
  const label cellI,
  const label faceI
)
{
  if (cellI == -1)
  {
    FATAL_ERROR_IN("wallBoundedStreamLineParticle::interpolateFields(..)")
      << "Cell:" << cellI << abort(FatalError);
  }
  const tetIndices ti = currentTetIndices();
  const vector U = td.vvInterp_[td.UIndex_].interpolate
  (
    position,
    ti,     //cellI,
    faceI
  );
  // Check if at different position
  if
  (
   !sampledPositions_.size()
  || magSqr(sampledPositions_.last()-position) > mousse::sqr(SMALL)
  )
  {
    // Store the location
    sampledPositions_.append(position);
    // Store the scalar fields
    sampledScalars_.setSize(td.vsInterp_.size());
    FOR_ALL(td.vsInterp_, scalarI)
    {
      sampledScalars_[scalarI].append
      (
        td.vsInterp_[scalarI].interpolate
        (
          position,
          ti,     //cellI,
          faceI
        )
      );
    }
    // Store the vector fields
    sampledVectors_.setSize(td.vvInterp_.size());
    FOR_ALL(td.vvInterp_, vectorI)
    {
      vector positionU;
      if (vectorI == td.UIndex_)
      {
        positionU = U;
      }
      else
      {
        positionU = td.vvInterp_[vectorI].interpolate
        (
          position,
          ti,     //cellI,
          faceI
        );
      }
      sampledVectors_[vectorI].append(positionU);
    }
  }
  return U;
}
mousse::vector mousse::wallBoundedStreamLineParticle::sample
(
  trackingData& td
)
{
  vector U = interpolateFields(td, position(), cell(), tetFace());
  if (!td.trackForward_)
  {
    U = -U;
  }
  scalar magU = mag(U);
  if (magU < SMALL)
  {
    // Stagnant particle. Might as well stop
    lifeTime_ = 0;
  }
  U /= magU;
  return U;
}
// Constructors 
mousse::wallBoundedStreamLineParticle::wallBoundedStreamLineParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label tetFaceI,
  const label tetPtI,
  const label meshEdgeStart,
  const label diagEdge,
  const label lifeTime
)
:
  wallBoundedParticle
  (
    mesh,
    position,
    cellI,
    tetFaceI,
    tetPtI,
    meshEdgeStart,
    diagEdge
  ),
  lifeTime_(lifeTime)
{}
mousse::wallBoundedStreamLineParticle::wallBoundedStreamLineParticle
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  wallBoundedParticle(mesh, is, readFields)
{
  if (readFields)
  {
    List<scalarList> sampledScalars;
    List<vectorList> sampledVectors;
    is  >> lifeTime_
      >> sampledPositions_ >> sampledScalars >> sampledVectors;
    sampledScalars_.setSize(sampledScalars.size());
    FOR_ALL(sampledScalars, i)
    {
      sampledScalars_[i].transfer(sampledScalars[i]);
    }
    sampledVectors_.setSize(sampledVectors.size());
    FOR_ALL(sampledVectors, i)
    {
      sampledVectors_[i].transfer(sampledVectors[i]);
    }
  }
  // Check state of Istream
  is.check
  (
    "wallBoundedStreamLineParticle::wallBoundedStreamLineParticle"
    "(const Cloud<wallBoundedStreamLineParticle>&, Istream&, bool)"
  );
}
mousse::wallBoundedStreamLineParticle::wallBoundedStreamLineParticle
(
  const wallBoundedStreamLineParticle& p
)
:
  wallBoundedParticle(p),
  lifeTime_(p.lifeTime_),
  sampledPositions_(p.sampledPositions_),
  sampledScalars_(p.sampledScalars_),
  sampledVectors_(p.sampledVectors_)
{}
// Member Functions 
bool mousse::wallBoundedStreamLineParticle::move
(
  trackingData& td,
  const scalar trackTime
)
{
  wallBoundedStreamLineParticle& p = static_cast
  <
    wallBoundedStreamLineParticle&
  >(*this);
  // Check position is inside tet
  //checkInside();
  td.switchProcessor = false;
  td.keepParticle = true;
  scalar tEnd = (1.0 - stepFraction())*trackTime;
  scalar maxDt = mesh_.bounds().mag();
  while
  (
    td.keepParticle
  && !td.switchProcessor
  && lifeTime_ > 0
  )
  {
    // set the lagrangian time-step
    scalar dt = maxDt;
    --lifeTime_;
    // Get sampled velocity and fields. Store if position changed.
    vector U = sample(td);
    // !user parameter!
    if (dt < SMALL)
    {
      // Force removal
      lifeTime_ = 0;
      break;
    }
    if (td.trackLength_ < GREAT)
    {
      dt = td.trackLength_;
    }
    scalar fraction = trackToEdge(td, position() + dt*U);
    dt *= fraction;
    tEnd -= dt;
    stepFraction() = 1.0 - tEnd/trackTime;
    if (tEnd <= ROOTVSMALL)
    {
      // Force removal
      lifeTime_ = 0;
    }
    if
    (
      !td.keepParticle
    ||  td.switchProcessor
    ||  lifeTime_ == 0
    )
    {
      break;
    }
  }
  if (!td.keepParticle || lifeTime_ == 0)
  {
    if (lifeTime_ == 0)
    {
      if (debug)
      {
        Pout<< "wallBoundedStreamLineParticle :"
          << " Removing stagnant particle:"
          << p.position()
          << " sampled positions:" << sampledPositions_.size()
          << endl;
      }
      td.keepParticle = false;
    }
    else
    {
      // Normal exit. Store last position and fields
      sample(td);
      if (debug)
      {
        Pout<< "wallBoundedStreamLineParticle : Removing particle:"
          << p.position()
          << " sampled positions:" << sampledPositions_.size()
          << endl;
      }
    }
    // Transfer particle data into trackingData.
    {
      //td.allPositions_.append(sampledPositions_);
      td.allPositions_.append(vectorList());
      vectorList& top = td.allPositions_.last();
      top.transfer(sampledPositions_);
    }
    FOR_ALL(sampledScalars_, i)
    {
      //td.allScalars_[i].append(sampledScalars_[i]);
      td.allScalars_[i].append(scalarList());
      scalarList& top = td.allScalars_[i].last();
      top.transfer(sampledScalars_[i]);
    }
    FOR_ALL(sampledVectors_, i)
    {
      //td.allVectors_[i].append(sampledVectors_[i]);
      td.allVectors_[i].append(vectorList());
      vectorList& top = td.allVectors_[i].last();
      top.transfer(sampledVectors_[i]);
    }
  }
  return td.keepParticle;
}
void mousse::wallBoundedStreamLineParticle::readFields
(
  Cloud<wallBoundedStreamLineParticle>& c
)
{
  if (!c.size())
  {
    return;
  }
  wallBoundedParticle::readFields(c);
  IOField<label> lifeTime
  (
    c.fieldIOobject("lifeTime", IOobject::MUST_READ)
  );
  c.checkFieldIOobject(c, lifeTime);
  vectorFieldIOField sampledPositions
  (
    c.fieldIOobject("sampledPositions", IOobject::MUST_READ)
  );
  c.checkFieldIOobject(c, sampledPositions);
  label i = 0;
  FOR_ALL_ITER(Cloud<wallBoundedStreamLineParticle>, c, iter)
  {
    iter().lifeTime_ = lifeTime[i];
    iter().sampledPositions_.transfer(sampledPositions[i]);
    i++;
  }
}
void mousse::wallBoundedStreamLineParticle::writeFields
(
  const Cloud<wallBoundedStreamLineParticle>& c
)
{
  wallBoundedParticle::writeFields(c);
  label np =  c.size();
  IOField<label> lifeTime
  (
    c.fieldIOobject("lifeTime", IOobject::NO_READ),
    np
  );
  vectorFieldIOField sampledPositions
  (
    c.fieldIOobject("sampledPositions", IOobject::NO_READ),
    np
  );
  label i = 0;
  FOR_ALL_CONST_ITER(Cloud<wallBoundedStreamLineParticle>, c, iter)
  {
    lifeTime[i] = iter().lifeTime_;
    sampledPositions[i] = iter().sampledPositions_;
    i++;
  }
  lifeTime.write();
  sampledPositions.write();
}
// IOstream Operators 
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const wallBoundedStreamLineParticle& p
)
{
  os  << static_cast<const wallBoundedParticle&>(p)
    << token::SPACE << p.lifeTime_
    << token::SPACE << p.sampledPositions_
    << token::SPACE << p.sampledScalars_
    << token::SPACE << p.sampledVectors_;
  // Check state of Ostream
  os.check
  (
    "Ostream& operator<<(Ostream&, const wallBoundedStreamLineParticle&)"
  );
  return os;
}
