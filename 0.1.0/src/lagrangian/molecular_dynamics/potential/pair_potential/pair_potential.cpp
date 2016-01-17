// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "pair_potential.hpp"
#include "energy_scaling_function.hpp"
// Static Data Members
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(pairPotential, 0);
  DEFINE_RUN_TIME_SELECTION_TABLE(pairPotential, dictionary);
}
// Private Member Functions 
void mousse::pairPotential::scaleEnergy(scalar& e, const scalar r) const
{
  if (!esfPtr_)
  {
    esfPtr_ = energyScalingFunction::New
    (
      name_, pairPotentialProperties_, *this
    ).ptr();
  }
  esfPtr_->scaleEnergy(e, r);
}
// Constructors 
mousse::pairPotential::pairPotential
(
  const word& name,
  const dictionary& pairPotentialProperties
)
:
  name_(name),
  pairPotentialProperties_(pairPotentialProperties),
  rCut_(readScalar(pairPotentialProperties_.lookup("rCut"))),
  rCutSqr_(rCut_*rCut_),
  rMin_(readScalar(pairPotentialProperties_.lookup("rMin"))),
  dr_(readScalar(pairPotentialProperties_.lookup("dr"))),
  forceLookup_(0),
  energyLookup_(0),
  esfPtr_(NULL),
  writeTables_(Switch(pairPotentialProperties_.lookup("writeTables")))
{}
// Member Functions 
void mousse::pairPotential::setLookupTables()
{
  label N = label((rCut_ - rMin_)/dr_) + 1;
  forceLookup_.setSize(N);
  energyLookup_.setSize(N);
  FOR_ALL(forceLookup_, k)
  {
    energyLookup_[k] = scaledEnergy(k*dr_ + rMin_);
    forceLookup_[k] = -energyDerivative((k*dr_ + rMin_), true);
  }
}
mousse::scalar mousse::pairPotential::force(const scalar r) const
{
  scalar k_rIJ = (r - rMin_)/dr_;
  label k = label(k_rIJ);
  if (k < 0)
  {
    FATAL_ERROR_IN("pairPotential::force(const scalar) const")
      << "r less than rMin in pair potential " << name_ << nl
      << abort(FatalError);
  }
  scalar f =
    (k_rIJ - k)*forceLookup_[k+1]
   + (k + 1 - k_rIJ)*forceLookup_[k];
  return f;
}
mousse::List< mousse::Pair< mousse::scalar > >
mousse::pairPotential::forceTable() const
{
  List<Pair<scalar> > forceTab(forceLookup_.size());
  FOR_ALL(forceLookup_,k)
  {
    forceTab[k].first() = rMin_ + k*dr_;
    forceTab[k].second() = forceLookup_[k];
  }
  return forceTab;
}
mousse::scalar mousse::pairPotential::energy(const scalar r) const
{
  scalar k_rIJ = (r - rMin_)/dr_;
  label k = label(k_rIJ);
  if (k < 0)
  {
    FATAL_ERROR_IN("pairPotential::energy(const scalar) const")
      << "r less than rMin in pair potential " << name_ << nl
      << abort(FatalError);
  }
  scalar e =
    (k_rIJ - k)*energyLookup_[k+1]
   + (k + 1 - k_rIJ)*energyLookup_[k];
  return e;
}
mousse::List< mousse::Pair< mousse::scalar > >
  mousse::pairPotential::energyTable() const
{
  List<Pair<scalar> > energyTab(energyLookup_.size());
  FOR_ALL(energyLookup_,k)
  {
    energyTab[k].first() = rMin_ + k*dr_;
    energyTab[k].second() = energyLookup_[k];
  }
  return energyTab;
}
mousse::scalar mousse::pairPotential::scaledEnergy(const scalar r) const
{
  scalar e = unscaledEnergy(r);
  scaleEnergy(e, r);
  return e;
}
mousse::scalar mousse::pairPotential::energyDerivative
(
  const scalar r,
  const bool scaledEnergyDerivative
) const
{
  // Local quadratic fit to energy: E = a0 + a1*r + a2*r^2
  // Differentiate to give f = -dE/dr = -a1 - 2*a2*r
  scalar ra = r - dr_;
  scalar rf = r;
  scalar rb = r + dr_;
  scalar Ea, Ef, Eb;
  if (scaledEnergyDerivative)
  {
    Ea = scaledEnergy(ra);
    Ef = scaledEnergy(rf);
    Eb = scaledEnergy(rb);
  }
  else
  {
    Ea = unscaledEnergy(ra);
    Ef = unscaledEnergy(rf);
    Eb = unscaledEnergy(rb);
  }
  scalar denominator = (ra - rf)*(ra - rb)*(rf - rb);
  scalar a1 =
  (
    rb*rb*(Ea - Ef) + ra*ra*(Ef - Eb) + rf*rf*(Eb - Ea)
  ) / denominator;
  scalar a2 =
  (
    rb*(Ef - Ea) + rf*(Ea - Eb) + ra*(Eb - Ef)
  ) / denominator;
  return a1 + 2.0*a2*r;
}
bool mousse::pairPotential::read(const dictionary& pairPotentialProperties)
{
  pairPotentialProperties_ = pairPotentialProperties;
  return true;
}
