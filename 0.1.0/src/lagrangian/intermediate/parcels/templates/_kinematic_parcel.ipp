// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_kinematic_parcel.hpp"
#include "force_su_sp.hpp"
#include "_integration_scheme.hpp"
#include "mesh_tools.hpp"


// Static Data Members
template<class ParcelType>
mousse::label mousse::KinematicParcel<ParcelType>::maxTrackAttempts = 1;


//  Protected Member Functions
template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::setCellValues
(
  TrackData& td,
  const scalar dt,
  const label cellI
)
{
  tetIndices tetIs = this->currentTetIndices();
  rhoc_ = td.rhoInterp().interpolate(this->position(), tetIs);
  if (rhoc_ < td.cloud().constProps().rhoMin()) {
    if (debug) {
      WARNING_IN
      (
        "void mousse::KinematicParcel<ParcelType>::setCellValues"
        "("
        "  TrackData&, "
        "  const scalar, "
        "  const label"
        ")"
      )
      << "Limiting observed density in cell " << cellI << " to "
      << td.cloud().constProps().rhoMin() <<  nl << endl;
    }
    rhoc_ = td.cloud().constProps().rhoMin();
  }
  Uc_ = td.UInterp().interpolate(this->position(), tetIs);
  muc_ = td.muInterp().interpolate(this->position(), tetIs);
  // Apply dispersion components to carrier phase velocity
  Uc_ =
    td.cloud().dispersion().update
    (
      dt,
      cellI,
      U_,
      Uc_,
      UTurb_,
      tTurb_
    );
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::cellValueSourceCorrection
(
  TrackData& td,
  const scalar /*dt*/,
  const label cellI
)
{
  Uc_ += td.cloud().UTrans()[cellI]/massCell(cellI);
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::calc
(
  TrackData& td,
  const scalar dt,
  const label cellI
)
{
  // Define local properties at beginning of time step
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  const scalar np0 = nParticle_;
  const scalar mass0 = mass();
  // Reynolds number
  const scalar Re = this->Re(U_, d_, rhoc_, muc_);
  // Sources
  //~~~~~~~~
  // Explicit momentum source for particle
  vector Su = vector::zero;
  // Linearised momentum source coefficient
  scalar Spu = 0.0;
  // Momentum transfer from the particle to the carrier phase
  vector dUTrans = vector::zero;
  // Motion
  // ~~~~~~
  // Calculate new particle velocity
  this->U_ = calcVelocity(td, dt, cellI, Re, muc_, mass0, Su, dUTrans, Spu);
  // Accumulate carrier phase source terms
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (td.cloud().solution().coupled()) {
    // Update momentum transfer
    td.cloud().UTrans()[cellI] += np0*dUTrans;
    // Update momentum transfer coefficient
    td.cloud().UCoeff()[cellI] += np0*Spu;
  }
}


template<class ParcelType>
template<class TrackData>
const mousse::vector mousse::KinematicParcel<ParcelType>::calcVelocity
(
  TrackData& td,
  const scalar dt,
  const label /*cellI*/,
  const scalar Re,
  const scalar mu,
  const scalar mass,
  const vector& Su,
  vector& dUTrans,
  scalar& Spu
) const
{
  typedef typename TrackData::cloudType cloudType;
  typedef typename cloudType::parcelType parcelType;
  typedef typename cloudType::forceType forceType;
  const forceType& forces = td.cloud().forces();
  // Momentum source due to particle forces
  const parcelType& p = static_cast<const parcelType&>(*this);
  const forceSuSp Fcp = forces.calcCoupled(p, dt, mass, Re, mu);
  const forceSuSp Fncp = forces.calcNonCoupled(p, dt, mass, Re, mu);
  const forceSuSp Feff = Fcp + Fncp;
  const scalar massEff = forces.massEff(p, mass);
  // New particle velocity
  //~~~~~~~~~~~~~~~~~~~~~~
  // Update velocity - treat as 3-D
  const vector abp = (Feff.Sp()*Uc_ + (Feff.Su() + Su))/massEff;
  const scalar bp = Feff.Sp()/massEff;
  Spu = dt*Feff.Sp();
  IntegrationScheme<vector>::integrationResult Ures =
    td.cloud().UIntegrator().integrate(U_, dt, abp, bp);
  vector Unew = Ures.value();
  // note: Feff.Sp() and Fc.Sp() must be the same
  dUTrans += dt*(Feff.Sp()*(Ures.average() - Uc_) - Fcp.Su());
  // Apply correction to velocity and dUTrans for reduced-D cases
  const polyMesh& mesh = td.cloud().pMesh();
  meshTools::constrainDirection(mesh, mesh.solutionD(), Unew);
  meshTools::constrainDirection(mesh, mesh.solutionD(), dUTrans);
  return Unew;
}


// Constructors 
template<class ParcelType>
mousse::KinematicParcel<ParcelType>::KinematicParcel
(
  const KinematicParcel<ParcelType>& p
)
:
  ParcelType{p},
  active_{p.active_},
  typeId_{p.typeId_},
  nParticle_{p.nParticle_},
  d_{p.d_},
  dTarget_{p.dTarget_},
  U_{p.U_},
  rho_{p.rho_},
  age_{p.age_},
  tTurb_{p.tTurb_},
  UTurb_{p.UTurb_},
  rhoc_{p.rhoc_},
  Uc_{p.Uc_},
  muc_{p.muc_}
{}


template<class ParcelType>
mousse::KinematicParcel<ParcelType>::KinematicParcel
(
  const KinematicParcel<ParcelType>& p,
  const polyMesh& mesh
)
:
  ParcelType{p, mesh},
  active_{p.active_},
  typeId_{p.typeId_},
  nParticle_{p.nParticle_},
  d_{p.d_},
  dTarget_{p.dTarget_},
  U_{p.U_},
  rho_{p.rho_},
  age_{p.age_},
  tTurb_{p.tTurb_},
  UTurb_{p.UTurb_},
  rhoc_{p.rhoc_},
  Uc_{p.Uc_},
  muc_{p.muc_}
{}


// Member Functions 
template<class ParcelType>
template<class TrackData>
bool mousse::KinematicParcel<ParcelType>::move
(
  TrackData& td,
  const scalar trackTime
)
{
  typename TrackData::cloudType::parcelType& p =
    static_cast<typename TrackData::cloudType::parcelType&>(*this);
  td.switchProcessor = false;
  td.keepParticle = true;
  const polyMesh& mesh = td.cloud().pMesh();
  const polyBoundaryMesh& pbMesh = mesh.boundaryMesh();
  const scalarField& cellLengthScale = td.cloud().cellLengthScale();
  const scalar maxCo = td.cloud().solution().maxCo();
  scalar tEnd = (1.0 - p.stepFraction())*trackTime;
  scalar dtMax = trackTime;
  if (td.cloud().solution().transient()) {
    dtMax *= maxCo;
  }
  bool tracking = true;
  label nTrackingStalled = 0;
  while (td.keepParticle && !td.switchProcessor && tEnd > ROOTVSMALL) {
    // Apply correction to position for reduced-D cases
    meshTools::constrainToMeshCentre(mesh, p.position());
    const point start{p.position()};
    // Set the Lagrangian time-step
    scalar dt = min(dtMax, tEnd);
    // Cache the parcel current cell as this will change if a face is hit
    const label cellI = p.cell();
    const scalar magU = mag(U_);
    if (p.active() && tracking && (magU > ROOTVSMALL)) {
      const scalar d = dt*magU;
      const scalar dCorr = min(d, maxCo*cellLengthScale[cellI]);
      dt *= dCorr/d*p.trackToFace(p.position() + dCorr*U_/magU, td);
    }
    tEnd -= dt;
    scalar newStepFraction = 1.0 - tEnd/trackTime;
    if (tracking) {
      if (mag(p.stepFraction() - newStepFraction)
          < particle::minStepFractionTol) {
        nTrackingStalled++;
        if (nTrackingStalled > maxTrackAttempts) {
          tracking = false;
        }
      } else {
        nTrackingStalled = 0;
      }
    }
    p.stepFraction() = newStepFraction;
    bool calcParcel = true;
    if (!tracking && td.cloud().solution().steadyState()) {
      calcParcel = false;
    }
    // Avoid problems with extremely small timesteps
    if ((dt > ROOTVSMALL) && calcParcel) {
      // Update cell based properties
      p.setCellValues(td, dt, cellI);
      if (td.cloud().solution().cellValueSourceCorrection()) {
        p.cellValueSourceCorrection(td, dt, cellI);
      }
      p.calc(td, dt, cellI);
    }
    if (p.onBoundary() && td.keepParticle) {
      if (isA<processorPolyPatch>(pbMesh[p.patch(p.face())])) {
        td.switchProcessor = true;
      }
    }
    p.age() += dt;
    td.cloud().functions().postMove(p, cellI, dt, start, td.keepParticle);
  }
  return td.keepParticle;
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::hitFace(TrackData& td)
{
  typename TrackData::cloudType::parcelType& p =
    static_cast<typename TrackData::cloudType::parcelType&>(*this);
  td.cloud().functions().postFace(p, p.face(), td.keepParticle);
}


template<class ParcelType>
void mousse::KinematicParcel<ParcelType>::hitFace(int& /*td*/)
{}


template<class ParcelType>
template<class TrackData>
bool mousse::KinematicParcel<ParcelType>::hitPatch
(
  const polyPatch& pp,
  TrackData& td,
  const label /*patchI*/,
  const scalar trackFraction,
  const tetIndices& tetIs
)
{
  typename TrackData::cloudType::parcelType& p =
    static_cast<typename TrackData::cloudType::parcelType&>(*this);
  // Invoke post-processing model
  td.cloud().functions().postPatch
  (
    p,
    pp,
    trackFraction,
    tetIs,
    td.keepParticle
  );
  // Invoke surface film model
  if (td.cloud().surfaceFilm().transferParcel(p, pp, td.keepParticle)) {
    // All interactions done
    return true;
  } else {
    // Invoke patch interaction model
    return
      td.cloud().patchInteraction().correct
      (
        p,
        pp,
        td.keepParticle,
        trackFraction,
        tetIs
      );
  }
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::hitProcessorPatch
(
  const processorPolyPatch&,
  TrackData& td
)
{
  td.switchProcessor = true;
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::hitWallPatch
(
  const wallPolyPatch&,
  TrackData&,
  const tetIndices&
)
{
  // Wall interactions handled by generic hitPatch function
}


template<class ParcelType>
template<class TrackData>
void mousse::KinematicParcel<ParcelType>::hitPatch
(
  const polyPatch&,
  TrackData& td
)
{
  td.keepParticle = false;
}


template<class ParcelType>
void mousse::KinematicParcel<ParcelType>::transformProperties(const tensor& T)
{
  ParcelType::transformProperties(T);
  U_ = transform(T, U_);
}


template<class ParcelType>
void mousse::KinematicParcel<ParcelType>::transformProperties
(
  const vector& separation
)
{
  ParcelType::transformProperties(separation);
}


template<class ParcelType>
mousse::scalar mousse::KinematicParcel<ParcelType>::wallImpactDistance
(
  const vector&
) const
{
  return 0.5*d_;
}

// IOStream operators 
#include "_kinematic_parcel_io.ipp"

