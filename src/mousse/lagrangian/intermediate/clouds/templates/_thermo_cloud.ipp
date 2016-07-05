// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_thermo_cloud.hpp"
#include "_thermo_parcel.hpp"
#include "_heat_transfer_model.hpp"


// Protected Member Functions 
template<class CloudType>
void mousse::ThermoCloud<CloudType>::setModels()
{
  heatTransferModel_.reset
  (
    HeatTransferModel<ThermoCloud<CloudType>>::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
  TIntegrator_.reset
  (
    scalarIntegrationScheme::New
    (
      "T",
      this->solution().integrationSchemes()
    ).ptr()
  );
  if (this->solution().coupled()) {
    this->subModelProperties().lookup("radiation") >> radiation_;
  }
  if (radiation_) {
    radAreaP_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radAreaP",
          this->db().time().timeName(),
          this->db(),
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", dimArea, 0.0}
      }
    );
    radT4_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radT4",
          this->db().time().timeName(),
          this->db(),
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", pow4(dimTemperature), 0.0}
      }
    );
    radAreaPT4_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radAreaPT4",
          this->db().time().timeName(),
          this->db(),
          IOobject::READ_IF_PRESENT,
          IOobject::AUTO_WRITE
        },
        this->mesh(),
        {"zero", sqr(dimLength)*pow4(dimTemperature), 0.0}
      }
    );
  }
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::cloudReset(ThermoCloud<CloudType>& c)
{
  CloudType::cloudReset(c);
  heatTransferModel_.reset(c.heatTransferModel_.ptr());
  TIntegrator_.reset(c.TIntegrator_.ptr());
  radiation_ = c.radiation_;
}


// Constructors 
template<class CloudType>
mousse::ThermoCloud<CloudType>::ThermoCloud
(
  const word& cloudName,
  const volScalarField& rho,
  const volVectorField& U,
  const dimensionedVector& g,
  const SLGThermo& thermo,
  bool readFields
)
:
  CloudType
  {
    cloudName,
    rho,
    U,
    thermo.thermo().mu(),
    g,
    false
  },
  thermoCloud{},
  cloudCopyPtr_{nullptr},
  constProps_{this->particleProperties()},
  thermo_{thermo},
  T_{thermo.thermo().T()},
  p_{thermo.thermo().p()},
  heatTransferModel_{nullptr},
  TIntegrator_{nullptr},
  radiation_{false},
  radAreaP_{nullptr},
  radT4_{nullptr},
  radAreaPT4_{nullptr},
  hsTrans_
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        this->name() + ":hsTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
      },
      this->mesh(),
      {"zero", dimEnergy, 0.0}
    }
  },
  hsCoeff_
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        this->name() + ":hsCoeff",
        this->db().time().timeName(),
        this->db(),
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
      },
      this->mesh(),
      {"zero", dimEnergy/dimTemperature, 0.0}
    }
  }
{
  if (this->solution().active()) {
    setModels();
    if (readFields) {
      parcelType::readFields(*this);
    }
  }
  if (this->solution().resetSourcesOnStartup()) {
    resetSourceTerms();
  }
}


template<class CloudType>
mousse::ThermoCloud<CloudType>::ThermoCloud
(
  ThermoCloud<CloudType>& c,
  const word& name
)
:
  CloudType{c, name},
  thermoCloud(),
  cloudCopyPtr_{nullptr},
  constProps_{c.constProps_},
  thermo_{c.thermo_},
  T_{c.T()},
  p_{c.p()},
  heatTransferModel_{c.heatTransferModel_->clone()},
  TIntegrator_{c.TIntegrator_->clone()},
  radiation_{c.radiation_},
  radAreaP_{nullptr},
  radT4_{nullptr},
  radAreaPT4_{nullptr},
  hsTrans_
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        this->name() + ":hsTrans",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      c.hsTrans()
    }
  },
  hsCoeff_
  {
    new DimensionedField<scalar, volMesh>
    {
      {
        this->name() + ":hsCoeff",
        this->db().time().timeName(),
        this->db(),
        IOobject::NO_READ,
        IOobject::NO_WRITE,
        false
      },
      c.hsCoeff()
    }
  }
{
  if (radiation_) {
    radAreaP_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radAreaP",
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        c.radAreaP()
      }
    );
    radT4_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radT4",
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        c.radT4()
      }
    );
    radAreaPT4_.reset
    (
      new DimensionedField<scalar, volMesh>
      {
        {
          this->name() + ":radAreaPT4",
          this->db().time().timeName(),
          this->db(),
          IOobject::NO_READ,
          IOobject::NO_WRITE,
          false
        },
        c.radAreaPT4()
      }
    );
  }
}


template<class CloudType>
mousse::ThermoCloud<CloudType>::ThermoCloud
(
  const fvMesh& mesh,
  const word& name,
  const ThermoCloud<CloudType>& c
)
:
  CloudType{mesh, name, c},
  thermoCloud(),
  cloudCopyPtr_{nullptr},
  constProps_(),
  thermo_{c.thermo()},
  T_{c.T()},
  p_{c.p()},
  heatTransferModel_{nullptr},
  TIntegrator_{nullptr},
  radiation_{false},
  radAreaP_{nullptr},
  radT4_{nullptr},
  radAreaPT4_{nullptr},
  hsTrans_{nullptr},
  hsCoeff_{nullptr}
{}


// Destructor 
template<class CloudType>
mousse::ThermoCloud<CloudType>::~ThermoCloud()
{}


// Member Functions 
template<class CloudType>
void mousse::ThermoCloud<CloudType>::setParcelThermoProperties
(
  parcelType& parcel,
  const scalar lagrangianDt
)
{
  CloudType::setParcelThermoProperties(parcel, lagrangianDt);
  parcel.T() = constProps_.T0();
  parcel.Cp() = constProps_.Cp0();
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::checkParcelProperties
(
  parcelType& parcel,
  const scalar lagrangianDt,
  const bool fullyDescribed
)
{
  CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::storeState()
{
  cloudCopyPtr_.reset
  (
    static_cast<ThermoCloud<CloudType>*>
    (
      clone(this->name() + "Copy").ptr()
    )
  );
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::restoreState()
{
  cloudReset(cloudCopyPtr_());
  cloudCopyPtr_.clear();
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::resetSourceTerms()
{
  CloudType::resetSourceTerms();
  hsTrans_->field() = 0.0;
  hsCoeff_->field() = 0.0;
  if (radiation_) {
    radAreaP_->field() = 0.0;
    radT4_->field() = 0.0;
    radAreaPT4_->field() = 0.0;
  }
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::relaxSources
(
  const ThermoCloud<CloudType>& cloudOldTime
)
{
  CloudType::relaxSources(cloudOldTime);
  this->relax(hsTrans_(), cloudOldTime.hsTrans(), "h");
  this->relax(hsCoeff_(), cloudOldTime.hsCoeff(), "h");
  if (radiation_) {
    this->relax(radAreaP_(), cloudOldTime.radAreaP(), "radiation");
    this->relax(radT4_(), cloudOldTime.radT4(), "radiation");
    this->relax(radAreaPT4_(), cloudOldTime.radAreaPT4(), "radiation");
  }
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::scaleSources()
{
  CloudType::scaleSources();
  this->scale(hsTrans_(), "h");
  this->scale(hsCoeff_(), "h");
  if (radiation_) {
    this->scale(radAreaP_(), "radiation");
    this->scale(radT4_(), "radiation");
    this->scale(radAreaPT4_(), "radiation");
  }
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::preEvolve()
{
  CloudType::preEvolve();
  this->pAmbient() = thermo_.thermo().p().average().value();
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::evolve()
{
  if (this->solution().canEvolve()) {
    typename parcelType::template
      TrackingData<ThermoCloud<CloudType>> td{*this};
    this->solve(td);
  }
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::autoMap(const mapPolyMesh& mapper)
{
  typedef typename particle::TrackingData<ThermoCloud<CloudType>> tdType;
  tdType td{*this};
  Cloud<parcelType>::template autoMap<tdType>(td, mapper);
  this->updateMesh();
}


template<class CloudType>
void mousse::ThermoCloud<CloudType>::info()
{
  CloudType::info();
  Info << "    Temperature min/max             = " << Tmin() << ", " << Tmax()
    << endl;
}
