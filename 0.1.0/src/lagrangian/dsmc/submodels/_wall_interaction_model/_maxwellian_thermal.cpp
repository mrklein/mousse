// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_maxwellian_thermal.hpp"
#include "constants.hpp"

using namespace mousse::constant;
// Constructors 
template<class CloudType>
mousse::MaxwellianThermal<CloudType>::MaxwellianThermal
(
  const dictionary& dict,
  CloudType& cloud
)
:
  WallInteractionModel<CloudType>(cloud)
{}
// Destructor 
template<class CloudType>
mousse::MaxwellianThermal<CloudType>::~MaxwellianThermal()
{}
// Member Functions 
template<class CloudType>
void mousse::MaxwellianThermal<CloudType>::correct
(
  typename CloudType::parcelType& p,
  const wallPolyPatch& wpp
)
{
  vector& U = p.U();
  scalar& Ei = p.Ei();
  label typeId = p.typeId();
  label wppIndex = wpp.index();
  label wppLocalFace = wpp.whichFace(p.face());
  vector nw = p.normal();
  nw /= mag(nw);
  // Normal velocity magnitude
  scalar U_dot_nw = U & nw;
  // Wall tangential velocity (flow direction)
  vector Ut = U - U_dot_nw*nw;
  CloudType& cloud(this->owner());
  Random& rndGen(cloud.rndGen());
  while (mag(Ut) < SMALL)
  {
    // If the incident velocity is parallel to the face normal, no
    // tangential direction can be chosen.  Add a perturbation to the
    // incoming velocity and recalculate.
    U = vector
    (
      U.x()*(0.8 + 0.2*rndGen.scalar01()),
      U.y()*(0.8 + 0.2*rndGen.scalar01()),
      U.z()*(0.8 + 0.2*rndGen.scalar01())
    );
    U_dot_nw = U & nw;
    Ut = U - U_dot_nw*nw;
  }
  // Wall tangential unit vector
  vector tw1 = Ut/mag(Ut);
  // Other tangential unit vector
  vector tw2 = nw^tw1;
  scalar T = cloud.boundaryT().boundaryField()[wppIndex][wppLocalFace];
  scalar mass = cloud.constProps(typeId).mass();
  direction iDof = cloud.constProps(typeId).internalDegreesOfFreedom();
  U =
    sqrt(physicoChemical::k.value()*T/mass)
   *(
      rndGen.GaussNormal()*tw1
     + rndGen.GaussNormal()*tw2
     - sqrt(-2.0*log(max(1 - rndGen.scalar01(), VSMALL)))*nw
    );
  U += cloud.boundaryU().boundaryField()[wppIndex][wppLocalFace];
  Ei = cloud.equipartitionInternalEnergy(T, iDof);
}
