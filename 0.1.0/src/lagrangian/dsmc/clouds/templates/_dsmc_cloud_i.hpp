// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constants.hpp"
using namespace mousse::constant;
using namespace mousse::constant::mathematical;
// Member Functions 
template<class ParcelType>
inline const mousse::word& mousse::DSMCCloud<ParcelType>::cloudName() const
{
  return cloudName_;
}
template<class ParcelType>
inline const mousse::fvMesh& mousse::DSMCCloud<ParcelType>::mesh() const
{
  return mesh_;
}
template<class ParcelType>
inline const mousse::IOdictionary&
mousse::DSMCCloud<ParcelType>::particleProperties() const
{
  return particleProperties_;
}
template<class ParcelType>
inline const mousse::List<mousse::word>&
mousse::DSMCCloud<ParcelType>::typeIdList() const
{
  return typeIdList_;
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::nParticle() const
{
  return nParticle_;
}
template<class ParcelType>
inline const mousse::List<mousse::DynamicList<ParcelType*> >&
mousse::DSMCCloud<ParcelType>::cellOccupancy() const
{
  return cellOccupancy_;
}
template<class ParcelType>
inline mousse::volScalarField& mousse::DSMCCloud<ParcelType>::sigmaTcRMax()
{
  return sigmaTcRMax_;
}
template<class ParcelType>
inline mousse::scalarField&
mousse::DSMCCloud<ParcelType>::collisionSelectionRemainder()
{
  return collisionSelectionRemainder_;
}
template<class ParcelType>
inline const mousse::List<typename ParcelType::constantProperties>&
mousse::DSMCCloud<ParcelType>::constProps() const
{
  return constProps_;
}
template<class ParcelType>
inline const typename ParcelType::constantProperties&
mousse::DSMCCloud<ParcelType>::constProps
(
  label typeId
) const
{
  if (typeId < 0 || typeId >= constProps_.size())
  {
    FatalErrorIn("mousse::DSMCCloud<ParcelType>::constProps(label typeId)")
      << "constantProperties for requested typeId index "
      << typeId << " do not exist" << nl
      << abort(FatalError);
  }
  return constProps_[typeId];
}
template<class ParcelType>
inline mousse::Random& mousse::DSMCCloud<ParcelType>::rndGen()
{
  return rndGen_;
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::qBF()
{
  return q_.boundaryField();
}
template<class ParcelType>
inline mousse::volVectorField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::fDBF()
{
  return fD_.boundaryField();
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::rhoNBF()
{
  return rhoN_.boundaryField();
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::rhoMBF()
{
  return rhoM_.boundaryField();
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::linearKEBF()
{
  return linearKE_.boundaryField();
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::internalEBF()
{
  return internalE_.boundaryField();
}
template<class ParcelType>
inline mousse::volScalarField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::iDofBF()
{
  return iDof_.boundaryField();
}
template<class ParcelType>
inline mousse::volVectorField::GeometricBoundaryField&
mousse::DSMCCloud<ParcelType>::momentumBF()
{
  return momentum_.boundaryField();
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::boundaryT() const
{
  return boundaryT_;
}
template<class ParcelType>
inline const mousse::volVectorField&
mousse::DSMCCloud<ParcelType>::boundaryU() const
{
  return boundaryU_;
}
template<class ParcelType>
inline const mousse::BinaryCollisionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::binaryCollision() const
{
  return binaryCollisionModel_;
}
template<class ParcelType>
inline mousse::BinaryCollisionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::binaryCollision()
{
  return binaryCollisionModel_();
}
template<class ParcelType>
inline const mousse::WallInteractionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::wallInteraction() const
{
  return wallInteractionModel_;
}
template<class ParcelType>
inline mousse::WallInteractionModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::wallInteraction()
{
  return wallInteractionModel_();
}
template<class ParcelType>
inline const mousse::InflowBoundaryModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::inflowBoundary() const
{
  return inflowBoundaryModel_;
}
template<class ParcelType>
inline mousse::InflowBoundaryModel<mousse::DSMCCloud<ParcelType> >&
mousse::DSMCCloud<ParcelType>::inflowBoundary()
{
  return inflowBoundaryModel_();
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::massInSystem() const
{
  scalar sysMass = 0.0;
  forAllConstIter(typename DSMCCloud<ParcelType>, *this, iter)
  {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps
    (
      p.typeId()
    );
    sysMass += cP.mass();
  }
  return nParticle_*sysMass;
}
template<class ParcelType>
inline mousse::vector mousse::DSMCCloud<ParcelType>::linearMomentumOfSystem() const
{
  vector linearMomentum(vector::zero);
  forAllConstIter(typename DSMCCloud<ParcelType>, *this, iter)
  {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps
    (
      p.typeId()
    );
    linearMomentum += cP.mass()*p.U();
  }
  return nParticle_*linearMomentum;
}
template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::linearKineticEnergyOfSystem() const
{
  scalar linearKineticEnergy = 0.0;
  forAllConstIter(typename DSMCCloud<ParcelType>, *this, iter)
  {
    const ParcelType& p = iter();
    const typename ParcelType::constantProperties& cP = constProps
    (
      p.typeId()
    );
    linearKineticEnergy += 0.5*cP.mass()*(p.U() & p.U());
  }
  return nParticle_*linearKineticEnergy;
}
template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::internalEnergyOfSystem() const
{
  scalar internalEnergy = 0.0;
  forAllConstIter(typename DSMCCloud<ParcelType>, *this, iter)
  {
    const ParcelType& p = iter();
    internalEnergy += p.Ei();
  }
  return nParticle_*internalEnergy;
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::maxwellianAverageSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return
    2.0*sqrt(2.0*physicoChemical::k.value()*temperature/(pi*mass));
}
template<class ParcelType>
inline mousse::scalarField mousse::DSMCCloud<ParcelType>::maxwellianAverageSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    2.0*sqrt(2.0*physicoChemical::k.value()*temperature/(pi*mass));
  return tfld();
}
template<class ParcelType>
inline mousse::scalar mousse::DSMCCloud<ParcelType>::maxwellianRMSSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return sqrt(3.0*physicoChemical::k.value()*temperature/mass);
}
template<class ParcelType>
inline mousse::scalarField mousse::DSMCCloud<ParcelType>::maxwellianRMSSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    sqrt(3.0*physicoChemical::k.value()*temperature/mass);
  return tfld();
}
template<class ParcelType>
inline mousse::scalar
mousse::DSMCCloud<ParcelType>::maxwellianMostProbableSpeed
(
  scalar temperature,
  scalar mass
) const
{
  return sqrt(2.0*physicoChemical::k.value()*temperature/mass);
}
template<class ParcelType>
inline mousse::scalarField
mousse::DSMCCloud<ParcelType>::maxwellianMostProbableSpeed
(
  scalarField temperature,
  scalar mass
) const
{
  tmp<scalarField> tfld =
    sqrt(2.0*physicoChemical::k.value()*temperature/mass);
  return tfld();
}
template<class ParcelType>
inline const mousse::volScalarField& mousse::DSMCCloud<ParcelType>::q() const
{
  return q_;
}
template<class ParcelType>
inline const mousse::volVectorField& mousse::DSMCCloud<ParcelType>::fD() const
{
  return fD_;
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::rhoN() const
{
  return rhoN_;
}
template<class ParcelType>
inline const mousse::volScalarField& mousse::DSMCCloud<ParcelType>::rhoM() const
{
  return rhoM_;
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::dsmcRhoN() const
{
  return dsmcRhoN_;
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::linearKE() const
{
  return linearKE_;
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::internalE() const
{
  return internalE_;
}
template<class ParcelType>
inline const mousse::volScalarField&
mousse::DSMCCloud<ParcelType>::iDof() const
{
  return iDof_;
}
template<class ParcelType>
inline const mousse::volVectorField& mousse::DSMCCloud<ParcelType>::momentum() const
{
  return momentum_;
}
template<class ParcelType>
inline void mousse::DSMCCloud<ParcelType>::clear()
{
  return IDLList<ParcelType>::clear();
}
