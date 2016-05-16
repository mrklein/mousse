// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "solid_particle_cloud.hpp"


// Static Data Members
namespace mousse {

DEFINE_TEMPLATE_TYPE_NAME_AND_DEBUG(Cloud<solidParticle>, 0);

}


// Member Functions 
bool mousse::solidParticle::move
(
  trackingData& td,
  const scalar trackTime
)
{
  td.switchProcessor = false;
  td.keepParticle = true;
  const polyBoundaryMesh& pbMesh = mesh_.boundaryMesh();
  scalar tEnd = (1.0 - stepFraction())*trackTime;
  scalar dtMax = tEnd;
  while (td.keepParticle && !td.switchProcessor && tEnd > SMALL) {
    if (debug) {
      Info
        << "Time = " << mesh_.time().timeName()
        << " trackTime = " << trackTime
        << " tEnd = " << tEnd
        << " steptFraction() = " << stepFraction() << endl;
    }
    // set the lagrangian time-step
    scalar dt = min(dtMax, tEnd);
    // remember which cell the parcel is in
    // since this will change if a face is hit
    label cellI = cell();
    dt *= trackToFace(position() + dt*U_, td);
    tEnd -= dt;
    stepFraction() = 1.0 - tEnd/trackTime;
    cellPointWeight cpw(mesh_, position(), cellI, face());
    scalar rhoc = td.rhoInterp().interpolate(cpw);
    vector Uc = td.UInterp().interpolate(cpw);
    scalar nuc = td.nuInterp().interpolate(cpw);
    scalar rhop = td.cloud().rhop();
    scalar magUr = mag(Uc - U_);
    scalar ReFunc = 1.0;
    scalar Re = magUr*d_/nuc;
    if (Re > 0.01) {
      ReFunc += 0.15*pow(Re, 0.687);
    }
    scalar Dc = (24.0*nuc/d_)*ReFunc*(3.0/4.0)*(rhoc/(d_*rhop));
    U_ = (U_ + dt*(Dc*Uc + (1.0 - rhoc/rhop)*td.g()))/(1.0 + dt*Dc);
    if (onBoundary() && td.keepParticle) {
      if (isA<processorPolyPatch>(pbMesh[patch(face())])) {
        td.switchProcessor = true;
      }
    }
  }
  return td.keepParticle;
}


bool mousse::solidParticle::hitPatch
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


void mousse::solidParticle::hitProcessorPatch
(
  const processorPolyPatch&,
  trackingData& td
)
{
  td.switchProcessor = true;
}


void mousse::solidParticle::hitWallPatch
(
  const wallPolyPatch&,
  trackingData& td,
  const tetIndices& tetIs
)
{
  vector nw = tetIs.faceTri(mesh_).normal();
  nw /= mag(nw);
  scalar Un = U_ & nw;
  vector Ut = U_ - Un*nw;
  if (Un > 0) {
    U_ -= (1.0 + td.cloud().e())*Un*nw;
  }
  U_ -= td.cloud().mu()*Ut;
}


void mousse::solidParticle::hitPatch
(
  const polyPatch&,
  trackingData& td
)
{
  td.keepParticle = false;
}


void mousse::solidParticle::transformProperties (const tensor& T)
{
  particle::transformProperties(T);
  U_ = transform(T, U_);
}


void mousse::solidParticle::transformProperties(const vector& separation)
{
  particle::transformProperties(separation);
}


mousse::scalar mousse::solidParticle::wallImpactDistance(const vector&) const
{
  return 0.5*d_;
}

