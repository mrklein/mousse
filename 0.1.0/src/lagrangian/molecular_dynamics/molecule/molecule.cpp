// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "molecule_cloud.hpp"
#include "molecule.hpp"
#include "random.hpp"
#include "time.hpp"


// Private Member Functions 
mousse::tensor mousse::molecule::rotationTensorX(scalar phi) const
{
  return
    {
      1,                0,                 0,
      0, mousse::cos(phi), -mousse::sin(phi),
      0, mousse::sin(phi),  mousse::cos(phi)
    };
}


mousse::tensor mousse::molecule::rotationTensorY(scalar phi) const
{
  return
    {
       mousse::cos(phi), 0, mousse::sin(phi),
       0,                1,                0,
      -mousse::sin(phi), 0, mousse::cos(phi)
    };
}


mousse::tensor mousse::molecule::rotationTensorZ(scalar phi) const
{
  return
    {
      mousse::cos(phi), -mousse::sin(phi), 0,
      mousse::sin(phi),  mousse::cos(phi), 0,
                     0,                 0, 1
    };
}


// Member Functions 
bool mousse::molecule::move(molecule::trackingData& td, const scalar trackTime)
{
  td.switchProcessor = false;
  td.keepParticle = true;
  const constantProperties& constProps(td.cloud().constProps(id_));
  if (td.part() == 0) {
    // First leapfrog velocity adjust part, required before tracking+force
    // part
    v_ += 0.5*trackTime*a_;
    pi_ += 0.5*trackTime*tau_;
  } else if (td.part() == 1) {
    // Leapfrog tracking part
    scalar tEnd = (1.0 - stepFraction())*trackTime;
    scalar dtMax = tEnd;
    while (td.keepParticle && !td.switchProcessor && tEnd > ROOTVSMALL) {
      // set the lagrangian time-step
      scalar dt = min(dtMax, tEnd);
      dt *= trackToFace(position() + dt*v_, td);
      tEnd -= dt;
      stepFraction() = 1.0 - tEnd/trackTime;
    }
  } else if (td.part() == 2) {
    // Leapfrog orientation adjustment, carried out before force calculation
    // but after tracking stage, i.e. rotation carried once linear motion
    // complete.
    if (!constProps.pointMolecule()) {
      const diagTensor& momentOfInertia(constProps.momentOfInertia());
      tensor R;
      if (!constProps.linearMolecule()) {
        R = rotationTensorX(0.5*trackTime*pi_.x()/momentOfInertia.xx());
        pi_ = pi_ & R;
        Q_ = Q_ & R;
      }
      R = rotationTensorY(0.5*trackTime*pi_.y()/momentOfInertia.yy());
      pi_ = pi_ & R;
      Q_ = Q_ & R;
      R = rotationTensorZ(trackTime*pi_.z()/momentOfInertia.zz());
      pi_ = pi_ & R;
      Q_ = Q_ & R;
      R = rotationTensorY(0.5*trackTime*pi_.y()/momentOfInertia.yy());
      pi_ = pi_ & R;
      Q_ = Q_ & R;
      if (!constProps.linearMolecule()) {
        R = rotationTensorX(0.5*trackTime*pi_.x()/momentOfInertia.xx());
        pi_ = pi_ & R;
        Q_ = Q_ & R;
      }
    }
    setSitePositions(constProps);
  } else if (td.part() == 3) {
    // Second leapfrog velocity adjust part, required after tracking+force
    // part
    scalar m = constProps.mass();
    a_ = vector::zero;
    tau_ = vector::zero;
    FOR_ALL(siteForces_, s) {
      const vector& f = siteForces_[s];
      a_ += f/m;
      tau_ += (constProps.siteReferencePositions()[s] ^ (Q_.T() & f));
    }
    v_ += 0.5*trackTime*a_;
    pi_ += 0.5*trackTime*tau_;
    if (constProps.pointMolecule()) {
      tau_ = vector::zero;
      pi_ = vector::zero;
    }
    if (constProps.linearMolecule()) {
      tau_.x() = 0.0;
      pi_.x() = 0.0;
    }
  } else {
    FATAL_ERROR_IN("molecule::move(trackingData&, const scalar)") << nl
      << td.part() << " is an invalid part of the integration method."
      << abort(FatalError);
  }
  return td.keepParticle;
}


void mousse::molecule::transformProperties(const tensor& T)
{
  particle::transformProperties(T);
  Q_ = T & Q_;
  v_ = transform(T, v_);
  a_ = transform(T, a_);
  pi_ = Q_.T() & transform(T, Q_ & pi_);
  tau_ = Q_.T() & transform(T, Q_ & tau_);
  rf_ = transform(T, rf_);
  sitePositions_ = position_ + (T & (sitePositions_ - position_));
  siteForces_ = T & siteForces_;
}


void mousse::molecule::transformProperties(const vector& separation)
{
  particle::transformProperties(separation);
  if (special_ == SPECIAL_TETHERED) {
    specialPosition_ += separation;
  }
  sitePositions_ = sitePositions_ + separation;
}


void mousse::molecule::setSitePositions(const constantProperties& constProps)
{
  sitePositions_ = position_ + (Q_ & constProps.siteReferencePositions());
}


void mousse::molecule::setSiteSizes(label size)
{
  sitePositions_.setSize(size);
  siteForces_.setSize(size);
}


bool mousse::molecule::hitPatch
(
  const polyPatch&,
  trackingData&,
  const label,
  const scalar,
  const tetIndices&
)
{
  return false;
}


void mousse::molecule::hitProcessorPatch
(
  const processorPolyPatch&,
  trackingData& td
)
{
  td.switchProcessor = true;
}


void mousse::molecule::hitWallPatch
(
  const wallPolyPatch&,
  trackingData&,
  const tetIndices& /*tetIs*/
)
{
  // Use of the normal from tetIs is not required as
  // hasWallImpactDistance for a moleculeCloud is false.
  vector nw = normal();
  nw /= mag(nw);
  scalar vn = v_ & nw;
  // Specular reflection
  if (vn > 0) {
    v_ -= 2*vn*nw;
  }
}


void mousse::molecule::hitPatch
(
  const polyPatch&,
  trackingData& td
)
{
  td.keepParticle = false;
}

