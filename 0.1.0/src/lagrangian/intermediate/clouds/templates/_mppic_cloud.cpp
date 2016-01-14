// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_mppic_cloud.hpp"
#include "_packing_model.hpp"
#include "particle_stress_model.hpp"
#include "_damping_model.hpp"
#include "_isotropy_model.hpp"
#include "time_scale_model.hpp"
// Protected Member Functions 
template<class CloudType>
void mousse::MPPICCloud<CloudType>::setModels()
{
  packingModel_.reset
  (
    PackingModel<MPPICCloud<CloudType> >::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
  dampingModel_.reset
  (
    DampingModel<MPPICCloud<CloudType> >::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
  isotropyModel_.reset
  (
    IsotropyModel<MPPICCloud<CloudType> >::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
}
// Constructors 
template<class CloudType>
mousse::MPPICCloud<CloudType>::MPPICCloud
(
  const word& cloudName,
  const volScalarField& rho,
  const volVectorField& U,
  const volScalarField& mu,
  const dimensionedVector& g,
  bool readFields
)
:
  CloudType(cloudName, rho, U, mu, g, false),
  packingModel_(NULL),
  dampingModel_(NULL),
  isotropyModel_(NULL)
{
  if (this->solution().steadyState())
  {
    FATAL_ERROR_IN
    (
      "mousse::MPPICCloud<CloudType>::MPPICCloud"
      "("
        "const word&, "
        "const volScalarField&, "
        "const volVectorField&, "
        "const volScalarField&, "
        "const dimensionedVector&, "
        "bool"
      ")"
    )   << "MPPIC modelling not available for steady state calculations"
      << exit(FatalError);
  }
  if (this->solution().active())
  {
    setModels();
    if (readFields)
    {
      parcelType::readFields(*this);
    }
  }
}
template<class CloudType>
mousse::MPPICCloud<CloudType>::MPPICCloud
(
  MPPICCloud<CloudType>& c,
  const word& name
)
:
  CloudType(c, name),
  packingModel_(c.packingModel_->clone()),
  dampingModel_(c.dampingModel_->clone()),
  isotropyModel_(c.isotropyModel_->clone())
{}
template<class CloudType>
mousse::MPPICCloud<CloudType>::MPPICCloud
(
  const fvMesh& mesh,
  const word& name,
  const MPPICCloud<CloudType>& c
)
:
  CloudType(mesh, name, c),
  packingModel_(NULL),
  dampingModel_(NULL),
  isotropyModel_(NULL)
{}
// Destructor 
template<class CloudType>
mousse::MPPICCloud<CloudType>::~MPPICCloud()
{}
// Member Functions 
template<class CloudType>
void mousse::MPPICCloud<CloudType>::storeState()
{
  cloudCopyPtr_.reset
  (
    static_cast<MPPICCloud<CloudType>*>
    (
      clone(this->name() + "Copy").ptr()
    )
  );
}
template<class CloudType>
void mousse::MPPICCloud<CloudType>::restoreState()
{
  this->cloudReset(cloudCopyPtr_());
  cloudCopyPtr_.clear();
}
template<class CloudType>
void mousse::MPPICCloud<CloudType>::evolve()
{
  if (this->solution().canEvolve())
  {
    typename parcelType::template
      TrackingData<MPPICCloud<CloudType> > td(*this);
    this->solve(td);
  }
}
template<class CloudType>
template<class TrackData>
void mousse::MPPICCloud<CloudType>::motion(TrackData& td)
{
  // Kinematic
  // ~~~~~~~~~
  // force calculation and tracking
  td.part() = TrackData::tpLinearTrack;
  CloudType::move(td, this->db().time().deltaTValue());
  // Preliminary
  // ~~~~~~~~~~~
  // switch forces off so they are not applied in corrector steps
  this->forces().setCalcNonCoupled(false);
  this->forces().setCalcCoupled(false);
  // Damping
  // ~~~~~~~
  if (dampingModel_->active())
  {
    // update averages
    td.updateAverages(*this);
    // memory allocation and eulerian calculations
    dampingModel_->cacheFields(true);
    // calculate the damping velocity corrections without moving the parcels
    td.part() = TrackData::tpDampingNoTrack;
    CloudType::move(td, this->db().time().deltaTValue());
    // correct the parcel positions and velocities
    td.part() = TrackData::tpCorrectTrack;
    CloudType::move(td, this->db().time().deltaTValue());
    // finalise and free memory
    dampingModel_->cacheFields(false);
  }
  // Packing
  // ~~~~~~~
  if (packingModel_->active())
  {
    // same procedure as for damping
    td.updateAverages(*this);
    packingModel_->cacheFields(true);
    td.part() = TrackData::tpPackingNoTrack;
    CloudType::move(td, this->db().time().deltaTValue());
    td.part() = TrackData::tpCorrectTrack;
    CloudType::move(td, this->db().time().deltaTValue());
    packingModel_->cacheFields(false);
  }
  // Isotropy
  // ~~~~~~~~
  if (isotropyModel_->active())
  {
    // update averages
    td.updateAverages(*this);
    // apply isotropy model
    isotropyModel_->calculate();
  }
  // Final
  // ~~~~~
  // update cell occupancy
  this->updateCellOccupancy();
  // switch forces back on
  this->forces().setCalcNonCoupled(true);
  this->forces().setCalcCoupled(this->solution().coupled());
}
template<class CloudType>
void mousse::MPPICCloud<CloudType>::info()
{
  CloudType::info();
  tmp<volScalarField> alpha = this->theta();
  const scalar alphaMin = gMin(alpha().internalField());
  const scalar alphaMax = gMax(alpha().internalField());
  Info<< "    Min cell volume fraction        = " << alphaMin << endl;
  Info<< "    Max cell volume fraction        = " << alphaMax << endl;
  if (alphaMax < SMALL)
  {
    return;
  }
  scalar nMin = GREAT;
  FOR_ALL(this->mesh().cells(), cellI)
  {
    const label n = this->cellOccupancy()[cellI].size();
    if (n > 0)
    {
      const scalar nPack = n*alphaMax/alpha()[cellI];
      if (nPack < nMin)
      {
        nMin = nPack;
      }
    }
  }
  reduce(nMin, minOp<scalar>());
  Info<< "    Min dense number of parcels     = " << nMin << endl;
}
