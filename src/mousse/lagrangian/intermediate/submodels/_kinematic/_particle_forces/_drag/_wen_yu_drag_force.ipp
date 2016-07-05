// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_wen_yu_drag_force.hpp"
#include "vol_fields.hpp"


// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::WenYuDragForce<CloudType>::CdRe(const scalar Re) const
{
  if (Re > 1000.0) {
    return 0.44*Re;
  } else {
    return 24.0*(1.0 + 0.15*pow(Re, 0.687));
  }
}


// Constructors 
template<class CloudType>
mousse::WenYuDragForce<CloudType>::WenYuDragForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>{owner, mesh, dict, typeName, true},
  alphac_
  {
    this->mesh().template lookupObject<volScalarField>
    (
      this->coeffs().lookup("alphac")
    )
  }
{}


template<class CloudType>
mousse::WenYuDragForce<CloudType>::WenYuDragForce
(
  const WenYuDragForce<CloudType>& df
)
:
  ParticleForce<CloudType>{df},
  alphac_
  {
    this->mesh().template lookupObject<volScalarField>
    (
      this->coeffs().lookup("alphac")
    )
  }
{}


// Destructor 
template<class CloudType>
mousse::WenYuDragForce<CloudType>::~WenYuDragForce()
{}


// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::WenYuDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  scalar alphac{alphac_[p.cell()]};
  return
    // forceSuSp
    {
      vector::zero,
      (mass/p.rho())*0.75*CdRe(alphac*Re)*muc
      *pow(alphac, -2.65)/(alphac*sqr(p.d()))
    };
}
