// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_reacting_cloud.hpp"
#include "_composition_model.hpp"
#include "_phase_change_model.hpp"


// Protected Member Functions 
template<class CloudType>
void mousse::ReactingCloud<CloudType>::setModels()
{
  compositionModel_.reset
  (
    CompositionModel<ReactingCloud<CloudType>>::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
  phaseChangeModel_.reset
  (
    PhaseChangeModel<ReactingCloud<CloudType>>::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::checkSuppliedComposition
(
  const scalarField& YSupplied,
  const scalarField& Y,
  const word& YName
)
{
  if (YSupplied.size() != Y.size()) {
    FATAL_ERROR_IN
    (
      "ReactingCloud<CloudType>::checkSuppliedComposition"
      "("
      "  const scalarField&, "
      "  const scalarField&, "
      "  const word&"
      ")"
    )
    << YName << " supplied, but size is not compatible with "
    << "parcel composition: " << nl << "    "
    << YName << "(" << YSupplied.size() << ") vs required composition "
    << YName << "(" << Y.size() << ")" << nl
    << abort(FatalError);
  }
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::cloudReset(ReactingCloud<CloudType>& c)
{
  CloudType::cloudReset(c);
  compositionModel_.reset(c.compositionModel_.ptr());
  phaseChangeModel_.reset(c.phaseChangeModel_.ptr());
}


// Constructors 
template<class CloudType>
mousse::ReactingCloud<CloudType>::ReactingCloud
(
  const word& cloudName,
  const volScalarField& rho,
  const volVectorField& U,
  const dimensionedVector& g,
  const SLGThermo& thermo,
  bool readFields
)
:
  CloudType{cloudName, rho, U, g, thermo, false},
  reactingCloud(),
  cloudCopyPtr_{nullptr},
  constProps_{this->particleProperties()},
  compositionModel_{nullptr},
  phaseChangeModel_{nullptr},
  rhoTrans_{thermo.carrier().species().size()}
{
  if (this->solution().active()) {
    setModels();
    if (readFields) {
      parcelType::readFields(*this, this->composition());
    }
  }
  // Set storage for mass source fields and initialise to zero
  FOR_ALL(rhoTrans_, i) {
    const word& specieName = thermo.carrier().species()[i];
    rhoTrans_.set
    (
      i,
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":rhoTrans_" + specieName,
          this->db().time().timeName(),
          this->db(),
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", dimMass, 0.0}
      }
    );
  }
  if (this->solution().resetSourcesOnStartup()) {
    resetSourceTerms();
  }
}


template<class CloudType>
mousse::ReactingCloud<CloudType>::ReactingCloud
(
  ReactingCloud<CloudType>& c,
  const word& name
)
:
  CloudType{c, name},
  reactingCloud(),
  cloudCopyPtr_{nullptr},
  constProps_{c.constProps_},
  compositionModel_{c.compositionModel_->clone()},
  phaseChangeModel_{c.phaseChangeModel_->clone()},
  rhoTrans_{c.rhoTrans_.size()}
{
  FOR_ALL(c.rhoTrans_, i) {
    const word& specieName = this->thermo().carrier().species()[i];
    rhoTrans_.set
    (
      i,
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":rhoTrans_" + specieName,
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        c.rhoTrans_[i]
      }
    );
  }
}


template<class CloudType>
mousse::ReactingCloud<CloudType>::ReactingCloud
(
  const fvMesh& mesh,
  const word& name,
  const ReactingCloud<CloudType>& c
)
:
  CloudType{mesh, name, c},
  reactingCloud(),
  cloudCopyPtr_{nullptr},
  constProps_(),
  compositionModel_{c.compositionModel_->clone()},
  phaseChangeModel_{nullptr},
  rhoTrans_{0}
{}


// Destructor 
template<class CloudType>
mousse::ReactingCloud<CloudType>::~ReactingCloud()
{}


// Member Functions 
template<class CloudType>
void mousse::ReactingCloud<CloudType>::setParcelThermoProperties
(
  parcelType& parcel,
  const scalar lagrangianDt
)
{
  CloudType::setParcelThermoProperties(parcel, lagrangianDt);
  parcel.pc() = this->thermo().thermo().p()[parcel.cell()];
  parcel.Y() = composition().YMixture0();
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::checkParcelProperties
(
  parcelType& parcel,
  const scalar lagrangianDt,
  const bool fullyDescribed
)
{
  CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);
  if (fullyDescribed) {
    checkSuppliedComposition
    (
      parcel.Y(),
      composition().YMixture0(),
      "YMixture"
    );
  }
  // derived information - store initial mass
  parcel.mass0() = parcel.mass();
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::storeState()
{
  cloudCopyPtr_.reset
  (
    static_cast<ReactingCloud<CloudType>*>
    (
      clone(this->name() + "Copy").ptr()
    )
  );
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::restoreState()
{
  cloudReset(cloudCopyPtr_());
  cloudCopyPtr_.clear();
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::resetSourceTerms()
{
  CloudType::resetSourceTerms();
  FOR_ALL(rhoTrans_, i) {
    rhoTrans_[i].field() = 0.0;
  }
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::relaxSources
(
  const ReactingCloud<CloudType>& cloudOldTime
)
{
  CloudType::relaxSources(cloudOldTime);
  typedef DimensionedField<scalar, volMesh> dsfType;
  FOR_ALL(rhoTrans_, fieldI) {
    dsfType& rhoT = rhoTrans_[fieldI];
    const dsfType& rhoT0 = cloudOldTime.rhoTrans()[fieldI];
    this->relax(rhoT, rhoT0, "rho");
  }
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::scaleSources()
{
  CloudType::scaleSources();
  typedef DimensionedField<scalar, volMesh> dsfType;
  FOR_ALL(rhoTrans_, fieldI) {
    dsfType& rhoT = rhoTrans_[fieldI];
    this->scale(rhoT, "rho");
  }
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::evolve()
{
  if (!this->solution().canEvolve())
    return;
  typename parcelType::template
    TrackingData<ReactingCloud<CloudType>> td{*this};
  this->solve(td);
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::autoMap(const mapPolyMesh& mapper)
{
  typedef typename particle::TrackingData<ReactingCloud<CloudType>> tdType;
  tdType td{*this};
  Cloud<parcelType>::template autoMap<tdType>(td, mapper);
  this->updateMesh();
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::info()
{
  CloudType::info();
  this->phaseChange().info(Info);
}


template<class CloudType>
void mousse::ReactingCloud<CloudType>::writeFields() const
{
  if (this->size()) {
    CloudType::particleType::writeFields(*this, this->composition());
  }
}

