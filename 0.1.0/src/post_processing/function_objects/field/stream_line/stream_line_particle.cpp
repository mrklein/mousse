// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "stream_line_particle.hpp"
#include "vector_field_io_field.hpp"


// Private Member Functions 
mousse::scalar mousse::streamLineParticle::calcSubCycleDeltaT
(
  trackingData& td,
  const scalar dt,
  const vector& U
) const
{
  particle testParticle{*this};
  bool oldKeepParticle = td.keepParticle;
  bool oldSwitchProcessor = td.switchProcessor;
  scalar fraction = testParticle.trackToFace(position()+dt*U, td);
  td.keepParticle = oldKeepParticle;
  td.switchProcessor = oldSwitchProcessor;
  // Adapt the dt to subdivide the trajectory into substeps.
  return dt*fraction/td.nSubCycle_;
}


mousse::vector mousse::streamLineParticle::interpolateFields
(
  const trackingData& td,
  const point& position,
  const label cellI,
  const label faceI
)
{
  if (cellI == -1) {
    FATAL_ERROR_IN("streamLineParticle::interpolateFields(..)")
      << "Cell:" << cellI << abort(FatalError);
  }
  sampledScalars_.setSize(td.vsInterp_.size());
  FOR_ALL(td.vsInterp_, scalarI) {
    sampledScalars_[scalarI].append
    (
      td.vsInterp_[scalarI].interpolate(position, cellI, faceI)
    );
  }
  sampledVectors_.setSize(td.vvInterp_.size());
  FOR_ALL(td.vvInterp_, vectorI) {
    sampledVectors_[vectorI].append
    (
      td.vvInterp_[vectorI].interpolate(position, cellI, faceI)
    );
  }
  const DynamicList<vector>& U = sampledVectors_[td.UIndex_];
  return U.last();
}


// Constructors 
mousse::streamLineParticle::streamLineParticle
(
  const polyMesh& mesh,
  const vector& position,
  const label cellI,
  const label lifeTime
)
:
  particle{mesh, position, cellI},
  lifeTime_{lifeTime}
{}


mousse::streamLineParticle::streamLineParticle
(
  const polyMesh& mesh,
  Istream& is,
  bool readFields
)
:
  particle{mesh, is, readFields}
{
  if (readFields) {
    List<scalarList> sampledScalars;
    List<vectorList> sampledVectors;
    is >> lifeTime_ >> sampledPositions_ >> sampledScalars
      >> sampledVectors;
    sampledScalars_.setSize(sampledScalars.size());
    FOR_ALL(sampledScalars, i) {
      sampledScalars_[i].transfer(sampledScalars[i]);
    }
    sampledVectors_.setSize(sampledVectors.size());
    FOR_ALL(sampledVectors, i) {
      sampledVectors_[i].transfer(sampledVectors[i]);
    }
  }
  // Check state of Istream
  is.check
  (
    "streamLineParticle::streamLineParticle"
    "(const Cloud<streamLineParticle>&, Istream&, bool)"
  );
}


mousse::streamLineParticle::streamLineParticle
(
  const streamLineParticle& p
)
:
  particle{p},
  lifeTime_{p.lifeTime_},
  sampledPositions_{p.sampledPositions_},
  sampledScalars_{p.sampledScalars_}
{}


// Member Functions 
bool mousse::streamLineParticle::move
(
  trackingData& td,
  const scalar trackTime
)
{
  streamLineParticle& p = static_cast<streamLineParticle&>(*this);
  td.switchProcessor = false;
  td.keepParticle = true;
  scalar tEnd = (1.0 - stepFraction())*trackTime;
  scalar maxDt = mesh_.bounds().mag();
  while (td.keepParticle && !td.switchProcessor && lifeTime_ > 0) {
    // set the lagrangian time-step
    scalar dt = maxDt;
    // Cross cell in steps:
    // - at subiter 0 calculate dt to cross cell in nSubCycle steps
    // - at the last subiter do all of the remaining track
    for (label subIter = 0; subIter < td.nSubCycle_; subIter++) {
      --lifeTime_;
      // Store current position and sampled velocity.
      sampledPositions_.append(position());
      vector U = interpolateFields(td, position(), cell(), face());
      if (!td.trackForward_) {
        U = -U;
      }
      scalar magU = mag(U);
      if (magU < SMALL) {
        // Stagnant particle. Might as well stop
        lifeTime_ = 0;
        break;
      }
      U /= magU;
      if (td.trackLength_ < GREAT) {
        dt = td.trackLength_;
      } else if (subIter == 0 && td.nSubCycle_ > 1) {
        // Adapt dt to cross cell in a few steps
        dt = calcSubCycleDeltaT(td, dt, U);
      } else if (subIter == td.nSubCycle_ - 1) {
        // Do full step on last subcycle
        dt = maxDt;
      }
      scalar fraction = trackToFace(position() + dt*U, td);
      dt *= fraction;
      tEnd -= dt;
      stepFraction() = 1.0 - tEnd/trackTime;
      if (tEnd <= ROOTVSMALL) {
        // Force removal
        lifeTime_ = 0;
      }
      if (face() != -1 || !td.keepParticle ||  td.switchProcessor
          ||  lifeTime_ == 0) {
        break;
      }
    }
  }
  if (!td.keepParticle || lifeTime_ == 0) {
    if (lifeTime_ == 0) {
      if (debug) {
        Pout << "streamLineParticle : Removing stagnant particle:"
          << p.position()
          << " sampled positions:" << sampledPositions_.size()
          << endl;
      }
      td.keepParticle = false;
    } else {
      // Normal exit. Store last position and fields
      sampledPositions_.append(position());
      interpolateFields(td, position(), cell(), face());
      if (debug) {
        Pout << "streamLineParticle : Removing particle:"
          << p.position()
          << " sampled positions:" << sampledPositions_.size()
          << endl;
      }
    }
    // Transfer particle data into trackingData.
    //td.allPositions_.append(sampledPositions_);
    td.allPositions_.append(vectorList());
    vectorList& top = td.allPositions_.last();
    top.transfer(sampledPositions_);
    FOR_ALL(sampledScalars_, i) {
      //td.allScalars_[i].append(sampledScalars_[i]);
      td.allScalars_[i].append(scalarList());
      scalarList& top = td.allScalars_[i].last();
      top.transfer(sampledScalars_[i]);
    }
    FOR_ALL(sampledVectors_, i) {
      //td.allVectors_[i].append(sampledVectors_[i]);
      td.allVectors_[i].append(vectorList());
      vectorList& top = td.allVectors_[i].last();
      top.transfer(sampledVectors_[i]);
    }
  }
  return td.keepParticle;
}


bool mousse::streamLineParticle::hitPatch
(
  const polyPatch&,
  trackingData&,
  const label /*patchI*/,
  const scalar /*trackFraction*/,
  const tetIndices& /*tetIs*/
)
{
  // Disable generic patch interaction
  return false;
}


void mousse::streamLineParticle::hitWedgePatch
(
  const wedgePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::hitSymmetryPlanePatch
(
  const symmetryPlanePolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::hitSymmetryPatch
(
  const symmetryPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::hitCyclicPatch
(
  const cyclicPolyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::hitProcessorPatch
(
  const processorPolyPatch&,
  trackingData& td
)
{
  // Switch particle
  td.switchProcessor = true;
}


void mousse::streamLineParticle::hitWallPatch
(
  const wallPolyPatch&,
  trackingData& td,
  const tetIndices&
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::hitPatch
(
  const polyPatch&,
  trackingData& td
)
{
  // Remove particle
  td.keepParticle = false;
}


void mousse::streamLineParticle::readFields(Cloud<streamLineParticle>& c)
{
  if (!c.size()) {
    return;
  }
  particle::readFields(c);
  IOField<label> lifeTime{c.fieldIOobject("lifeTime", IOobject::MUST_READ)};
  c.checkFieldIOobject(c, lifeTime);
  vectorFieldIOField sampledPositions
  {
    c.fieldIOobject("sampledPositions", IOobject::MUST_READ)
  };
  c.checkFieldIOobject(c, sampledPositions);
  label i = 0;
  FOR_ALL_ITER(Cloud<streamLineParticle>, c, iter) {
    iter().lifeTime_ = lifeTime[i];
    iter().sampledPositions_.transfer(sampledPositions[i]);
    i++;
  }
}


void mousse::streamLineParticle::writeFields(const Cloud<streamLineParticle>& c)
{
  particle::writeFields(c);
  label np =  c.size();
  IOField<label> lifeTime
  {
    c.fieldIOobject("lifeTime", IOobject::NO_READ),
    np
  };
  vectorFieldIOField sampledPositions
  {
    c.fieldIOobject("sampledPositions", IOobject::NO_READ),
    np
  };
  label i = 0;
  FOR_ALL_CONST_ITER(Cloud<streamLineParticle>, c, iter) {
    lifeTime[i] = iter().lifeTime_;
    sampledPositions[i] = iter().sampledPositions_;
    i++;
  }
  lifeTime.write();
  sampledPositions.write();
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const streamLineParticle& p)
{
  os << static_cast<const particle&>(p)
    << token::SPACE << p.lifeTime_
    << token::SPACE << p.sampledPositions_
    << token::SPACE << p.sampledScalars_
    << token::SPACE << p.sampledVectors_;
  // Check state of Ostream
  os.check("Ostream& operator<<(Ostream&, const streamLineParticle&)");
  return os;
}

