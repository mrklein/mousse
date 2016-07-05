// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_brownian_motion_force.hpp"
#include "mathematical_constants.hpp"
#include "demand_driven_data.hpp"
#include "turbulence_model.hpp"


using namespace mousse::constant;


// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::BrownianMotionForce<CloudType>::erfInv(const scalar y) const
{
  const scalar a = 0.147;
  scalar k = 2.0/(mathematical::pi*a) +  0.5*log(1.0 - y*y);
  scalar h = log(1.0 - y*y)/a;
  scalar x = sqrt(-k + sqrt(k*k - h));
  if (y < 0.0) {
    return -x;
  } else {
    return x;
  }
}


template<class CloudType>
mousse::tmp<mousse::volScalarField>
mousse::BrownianMotionForce<CloudType>::kModel() const
{
  const objectRegistry& obr = this->owner().mesh();
  const word turbName =
    IOobject::groupName
    (
      turbulenceModel::propertiesName,
      this->owner().U().group()
    );
  if (obr.foundObject<turbulenceModel>(turbName)) {
    const turbulenceModel& model =
      obr.lookupObject<turbulenceModel>(turbName);
    return model.k();
  } else {
    FATAL_ERROR_IN
    (
      "mousse::tmp<mousse::volScalarField>"
      "mousse::DispersionRASModel<CloudType>::kModel() const"
    )
    << "Turbulence model not found in mesh database" << nl
    << "Database objects include: " << obr.sortedToc()
    << abort(FatalError);
    return tmp<volScalarField>{nullptr};
  }
}


// Constructors 
template<class CloudType>
mousse::BrownianMotionForce<CloudType>::BrownianMotionForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>{owner, mesh, dict, typeName, true},
  rndGen_{owner.rndGen()},
  lambda_{readScalar(this->coeffs().lookup("lambda"))},
  turbulence_{readBool(this->coeffs().lookup("turbulence"))},
  kPtr_{nullptr},
  ownK_{false}
{}


template<class CloudType>
mousse::BrownianMotionForce<CloudType>::BrownianMotionForce
(
  const BrownianMotionForce& bmf
)
:
  ParticleForce<CloudType>{bmf},
  rndGen_{bmf.rndGen_},
  lambda_{bmf.lambda_},
  turbulence_{bmf.turbulence_},
  kPtr_{nullptr},
  ownK_{false}
{}


// Destructor 
template<class CloudType>
mousse::BrownianMotionForce<CloudType>::~BrownianMotionForce()
{}


// Member Functions 
template<class CloudType>
void mousse::BrownianMotionForce<CloudType>::cacheFields(const bool store)
{
  if (!turbulence_)
    return;
  if (store) {
    tmp<volScalarField> tk = kModel();
    if (tk.isTmp()) {
      kPtr_ = tk.ptr();
      ownK_ = true;
    } else {
      kPtr_ = tk.operator->();
      ownK_ = false;
    }
  } else {
    if (ownK_ && kPtr_) {
      deleteDemandDrivenData(kPtr_);
      ownK_ = false;
    }
  }
}


template<class CloudType>
mousse::forceSuSp mousse::BrownianMotionForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar /*Re*/,
  const scalar muc
) const
{
  forceSuSp value{vector::zero, 0.0};
  const scalar dp = p.d();
  const scalar Tc = p.Tc();
  const scalar eta = rndGen_.sample01<scalar>();
  const scalar alpha = 2.0*lambda_/dp;
  const scalar cc = 1.0 + alpha*(1.257 + 0.4*exp(-1.1/alpha));
  const scalar sigma = physicoChemical::sigma.value();
  scalar f = 0.0;
  if (turbulence_) {
    const label cellI = p.cell();
    const volScalarField& k = *kPtr_;
    const scalar kc = k[cellI];
    const scalar Dp = sigma*Tc*cc/(3*mathematical::pi*muc*dp);
    f = eta/mass*sqrt(2.0*sqr(kc)*sqr(Tc)/(Dp*dt));
  } else {
    const scalar rhoRatio = p.rho()/p.rhoc();
    const scalar s0 =
      216*muc*sigma*Tc/(sqr(mathematical::pi)*pow5(dp)*(rhoRatio)*cc);
    f = eta*sqrt(mathematical::pi*s0/dt);
  }
  const scalar sqrt2 = sqrt(2.0);
  for (label i = 0; i < 3; i++) {
    const scalar x = rndGen_.sample01<scalar>();
    const scalar eta = sqrt2*erfInv(2*x - 1.0);
    value.Su()[i] = mass*f*eta;
  }
  return value;
}

