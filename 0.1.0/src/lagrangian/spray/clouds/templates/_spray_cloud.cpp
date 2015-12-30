// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_spray_cloud.hpp"
#include "_atomization_model.hpp"
#include "_breakup_model.hpp"
// Protected Member Functions 
template<class CloudType>
void mousse::SprayCloud<CloudType>::setModels()
{
  atomizationModel_.reset
  (
    AtomizationModel<SprayCloud<CloudType> >::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
  breakupModel_.reset
  (
    BreakupModel<SprayCloud<CloudType> >::New
    (
      this->subModelProperties(),
      *this
    ).ptr()
  );
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::cloudReset
(
  SprayCloud<CloudType>& c
)
{
  CloudType::cloudReset(c);
  atomizationModel_.reset(c.atomizationModel_.ptr());
  breakupModel_.reset(c.breakupModel_.ptr());
}
// Constructors 
template<class CloudType>
mousse::SprayCloud<CloudType>::SprayCloud
(
  const word& cloudName,
  const volScalarField& rho,
  const volVectorField& U,
  const dimensionedVector& g,
  const SLGThermo& thermo,
  bool readFields
)
:
  CloudType(cloudName, rho, U, g, thermo, false),
  sprayCloud(),
  cloudCopyPtr_(NULL),
  averageParcelMass_(0.0),
  atomizationModel_(NULL),
  breakupModel_(NULL)
{
  if (this->solution().active())
  {
    setModels();
    averageParcelMass_ = this->injectors().averageParcelMass();
    if (readFields)
    {
      parcelType::readFields(*this, this->composition());
    }
    Info << "Average parcel mass: " << averageParcelMass_ << endl;
  }
  if (this->solution().resetSourcesOnStartup())
  {
    CloudType::resetSourceTerms();
  }
}
template<class CloudType>
mousse::SprayCloud<CloudType>::SprayCloud
(
  SprayCloud<CloudType>& c,
  const word& name
)
:
  CloudType(c, name),
  sprayCloud(),
  cloudCopyPtr_(NULL),
  averageParcelMass_(c.averageParcelMass_),
  atomizationModel_(c.atomizationModel_->clone()),
  breakupModel_(c.breakupModel_->clone())
{}
template<class CloudType>
mousse::SprayCloud<CloudType>::SprayCloud
(
  const fvMesh& mesh,
  const word& name,
  const SprayCloud<CloudType>& c
)
:
  CloudType(mesh, name, c),
  sprayCloud(),
  cloudCopyPtr_(NULL),
  averageParcelMass_(0.0),
  atomizationModel_(NULL),
  breakupModel_(NULL)
{}
// Destructor 
template<class CloudType>
mousse::SprayCloud<CloudType>::~SprayCloud()
{}
// Member Functions 
template<class CloudType>
void mousse::SprayCloud<CloudType>::setParcelThermoProperties
(
  parcelType& parcel,
  const scalar lagrangianDt
)
{
  CloudType::setParcelThermoProperties(parcel, lagrangianDt);
  const liquidMixtureProperties& liqMix = this->composition().liquids();
  const scalarField& Y(parcel.Y());
  scalarField X(liqMix.X(Y));
  // override rho and Cp from constantProperties
  parcel.Cp() = liqMix.Cp(parcel.pc(), parcel.T(), X);
  parcel.rho() = liqMix.rho(parcel.pc(), parcel.T(), X);
  parcel.sigma() = liqMix.sigma(parcel.pc(), parcel.T(), X);
  parcel.mu() = liqMix.mu(parcel.pc(), parcel.T(), X);
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::checkParcelProperties
(
  parcelType& parcel,
  const scalar lagrangianDt,
  const bool fullyDescribed
)
{
  CloudType::checkParcelProperties(parcel, lagrangianDt, fullyDescribed);
  // store the injection position and initial drop size
  parcel.position0() = parcel.position();
  parcel.d0() = parcel.d();
  parcel.y() = breakup().y0();
  parcel.yDot() = breakup().yDot0();
  parcel.liquidCore() = atomization().initLiquidCore();
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::storeState()
{
  cloudCopyPtr_.reset
  (
    static_cast<SprayCloud<CloudType>*>
    (
      clone(this->name() + "Copy").ptr()
    )
  );
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::restoreState()
{
  cloudReset(cloudCopyPtr_());
  cloudCopyPtr_.clear();
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::evolve()
{
  if (this->solution().canEvolve())
  {
    typename parcelType::template
      TrackingData<SprayCloud<CloudType> > td(*this);
    this->solve(td);
  }
}
template<class CloudType>
void mousse::SprayCloud<CloudType>::info()
{
  CloudType::info();
  scalar d32 = 1.0e+6*this->Dij(3, 2);
  scalar d10 = 1.0e+6*this->Dij(1, 0);
  scalar dMax = 1.0e+6*this->Dmax();
  scalar pen = this->penetration(0.95);
  Info << "    D10, D32, Dmax (mu)             = " << d10 << ", " << d32
    << ", " << dMax << nl
    << "    Liquid penetration 95% mass (m) = " << pen << endl;
}
