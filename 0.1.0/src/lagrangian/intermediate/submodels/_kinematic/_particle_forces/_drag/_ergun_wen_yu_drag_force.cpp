// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_ergun_wen_yu_drag_force.hpp"
#include "vol_fields.hpp"
// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::ErgunWenYuDragForce<CloudType>::CdRe
(
  const scalar Re
) const
{
  if (Re > 1000.0)
  {
    return 0.44*Re;
  }
  else
  {
    return 24.0*(1.0 + 0.15*pow(Re, 0.687));
  }
}
// Constructors 
template<class CloudType>
mousse::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
(
  CloudType& owner,
  const fvMesh& mesh,
  const dictionary& dict
)
:
  ParticleForce<CloudType>(owner, mesh, dict, typeName, true),
  alphac_
  (
    this->mesh().template lookupObject<volScalarField>
    (
      this->coeffs().lookup("alphac")
    )
  )
{}
template<class CloudType>
mousse::ErgunWenYuDragForce<CloudType>::ErgunWenYuDragForce
(
  const ErgunWenYuDragForce<CloudType>& df
)
:
  ParticleForce<CloudType>(df),
  alphac_
  (
    this->mesh().template lookupObject<volScalarField>
    (
      this->coeffs().lookup("alphac")
    )
  )
{}
// Destructor 
template<class CloudType>
mousse::ErgunWenYuDragForce<CloudType>::~ErgunWenYuDragForce()
{}
// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::ErgunWenYuDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar dt,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  scalar alphac(alphac_[p.cell()]);
  if (alphac < 0.8)
  {
    return forceSuSp
    (
      vector::zero,
      (mass/p.rho())
     *(150.0*(1.0 - alphac)/alphac + 1.75*Re)*muc/(alphac*sqr(p.d()))
    );
  }
  else
  {
    return forceSuSp
    (
      vector::zero,
      (mass/p.rho())
     *0.75*CdRe(alphac*Re)*muc*pow(alphac, -2.65)/(alphac*sqr(p.d()))
    );
  }
}
