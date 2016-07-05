// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_colliding_cloud.hpp"
#include "_collision_model.hpp"


// Protected Member Functions 
template<class CloudType>
void mousse::CollidingCloud<CloudType>::setModels()
{
  collisionModel_.reset
  (
    CollisionModel<CollidingCloud<CloudType>>::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
}


template<class CloudType>
template<class TrackData>
void  mousse::CollidingCloud<CloudType>::moveCollide
(
  TrackData& td,
  const scalar deltaT
)
{
  td.part() = TrackData::tpVelocityHalfStep;
  CloudType::move(td,  deltaT);
  td.part() = TrackData::tpLinearTrack;
  CloudType::move(td,  deltaT);
  // td.part() = TrackData::tpRotationalTrack;
  // CloudType::move(td);
  this->updateCellOccupancy();
  this->collision().collide();
  td.part() = TrackData::tpVelocityHalfStep;
  CloudType::move(td,  deltaT);
}


template<class CloudType>
void mousse::CollidingCloud<CloudType>::cloudReset(CollidingCloud<CloudType>& c)
{
  CloudType::cloudReset(c);
  collisionModel_.reset(c.collisionModel_.ptr());
}


// Constructors 
template<class CloudType>
mousse::CollidingCloud<CloudType>::CollidingCloud
(
  const word& cloudName,
  const volScalarField& rho,
  const volVectorField& U,
  const volScalarField& mu,
  const dimensionedVector& g,
  bool readFields
)
:
  CloudType{cloudName, rho, U, mu, g, false},
  constProps_{this->particleProperties()},
  collisionModel_{NULL}
{
  if (this->solution().steadyState()) {
    FATAL_ERROR_IN
    (
      "mousse::CollidingCloud<CloudType>::CollidingCloud"
      "("
        "const word&, "
        "const volScalarField&, "
        "const volVectorField&, "
        "const volScalarField&, "
        "const dimensionedVector&, "
        "bool"
      ")"
    )
    << "Collision modelling not currently available for steady state "
    << "calculations" << exit(FatalError);
  }
  if (this->solution().active()) {
    setModels();
    if (readFields) {
      parcelType::readFields(*this);
    }
  }
}


template<class CloudType>
mousse::CollidingCloud<CloudType>::CollidingCloud
(
  CollidingCloud<CloudType>& c,
  const word& name
)
:
  CloudType{c, name},
  collisionModel_{c.collisionModel_->clone()}
{}


template<class CloudType>
mousse::CollidingCloud<CloudType>::CollidingCloud
(
  const fvMesh& mesh,
  const word& name,
  const CollidingCloud<CloudType>& c
)
:
  CloudType{mesh, name, c},
  collisionModel_{NULL}
{}


// Destructor 
template<class CloudType>
mousse::CollidingCloud<CloudType>::~CollidingCloud()
{}


// Member Functions 
template<class CloudType>
bool mousse::CollidingCloud<CloudType>::hasWallImpactDistance() const
{
  return !collision().controlsWallInteraction();
}


template<class CloudType>
void mousse::CollidingCloud<CloudType>::storeState()
{
  cloudCopyPtr_.reset
  (
    static_cast<CollidingCloud<CloudType>*>
    (
      clone(this->name() + "Copy").ptr()
    )
  );
}


template<class CloudType>
void mousse::CollidingCloud<CloudType>::restoreState()
{
  cloudReset(cloudCopyPtr_());
  cloudCopyPtr_.clear();
}


template<class CloudType>
void mousse::CollidingCloud<CloudType>::evolve()
{
  if (this->solution().canEvolve()) {
    typename parcelType::template
      TrackingData<CollidingCloud<CloudType>> td(*this);
    this->solve(td);
  }
}


template<class CloudType>
template<class TrackData>
void  mousse::CollidingCloud<CloudType>::motion(TrackData& td)
{
  // Sympletic leapfrog integration of particle forces:
  // + apply half deltaV with stored force
  // + move positions with new velocity
  // + calculate forces in new position
  // + apply half deltaV with new force
  label nSubCycles = collision().nSubCycles();
  if (nSubCycles > 1) {
    Info << "    " << nSubCycles << " move-collide subCycles" << endl;
    subCycleTime moveCollideSubCycle
    (
      const_cast<Time&>(this->db().time()),
      nSubCycles
    );
    while(!(++moveCollideSubCycle).end()) {
      moveCollide(td, this->db().time().deltaTValue());
    }
    moveCollideSubCycle.endSubCycle();
  } else {
    moveCollide(td, this->db().time().deltaTValue());
  }
}


template<class CloudType>
void mousse::CollidingCloud<CloudType>::info()
{
  CloudType::info();
  scalar rotationalKineticEnergy = rotationalKineticEnergyOfSystem();
  reduce(rotationalKineticEnergy, sumOp<scalar>());
  Info << "    Rotational kinetic energy       = "
    << rotationalKineticEnergy << nl;
}

