// mousse: CFD toolbox
// Copyright (C) 2013-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_plessis_masliyah_drag_force.hpp"
#include "vol_fields.hpp"
// Private Member Functions 
template<class CloudType>
mousse::scalar mousse::PlessisMasliyahDragForce<CloudType>::CdRe
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
mousse::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
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
mousse::PlessisMasliyahDragForce<CloudType>::PlessisMasliyahDragForce
(
  const PlessisMasliyahDragForce<CloudType>& df
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
mousse::PlessisMasliyahDragForce<CloudType>::~PlessisMasliyahDragForce()
{}
// Member Functions 
template<class CloudType>
mousse::forceSuSp mousse::PlessisMasliyahDragForce<CloudType>::calcCoupled
(
  const typename CloudType::parcelType& p,
  const scalar /*dt*/,
  const scalar mass,
  const scalar Re,
  const scalar muc
) const
{
  scalar alphac(alphac_[p.cell()]);
  scalar cbrtAlphap(pow(1.0 - alphac, 1.0/3.0));
  scalar A =
    26.8*pow3(alphac)
   /(
      sqr(cbrtAlphap)
     *(1.0 - cbrtAlphap)
     *sqr(1.0 - sqr(cbrtAlphap))
     + SMALL
    );
  scalar B =
    sqr(alphac)
   /sqr(1.0 - sqr(cbrtAlphap));
  return forceSuSp
  (
    vector::zero,
    (mass/p.rho())
   *(A*(1.0 - alphac)/alphac + B*Re)*muc/(alphac*sqr(p.d()))
  );
}
