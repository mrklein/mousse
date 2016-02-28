// mousse: CFD toolbox
// Copyright (C) 2014-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "phase_pair.hpp"
// Private Member Functions 
mousse::tmp<mousse::volScalarField> mousse::phasePair::EoH
(
  const volScalarField& d
) const
{
  return mag(dispersed().rho() - continuous().rho())*mag(g())*sqr(d)/sigma();
}
// Constructors 
mousse::phasePair::phasePair
(
  const phaseModel& phase1,
  const phaseModel& phase2,
  const dimensionedVector& g,
  const scalarTable& sigmaTable,
  const bool ordered
)
:
  phasePairKey{phase1.name(), phase2.name(), ordered},
  phase1_{phase1},
  phase2_{phase2},
  g_{g},
  sigma_
  {
    "sigma",
    {1, 0, -2, 0, 0},
    sigmaTable
    [
      phasePairKey
      {
        phase1.name(),
        phase2.name(),
        false
      }
    ]
  }
{}
// Destructor 
mousse::phasePair::~phasePair()
{}
// Member Functions 
const mousse::phaseModel& mousse::phasePair::dispersed() const
{
  FATAL_ERROR_IN("mousse::phasePair::dispersed() const")
    << "Requested dispersed phase from an unordered pair."
    << exit(FatalError);
  return phase1_;
}
const mousse::phaseModel& mousse::phasePair::continuous() const
{
  FATAL_ERROR_IN("mousse::phasePair::dispersed() const")
    << "Requested continuous phase from an unordered pair."
    << exit(FatalError);
  return phase1_;
}
mousse::word mousse::phasePair::name() const
{
  word name2{phase2().name()};
  name2[0] = toupper(name2[0]);
  return phase1().name() + "And" + name2;
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::rho() const
{
  return phase1()*phase1().rho() + phase2()*phase2().rho();
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::magUr() const
{
  return mag(phase1().U() - phase2().U());
}
mousse::tmp<mousse::volVectorField> mousse::phasePair::Ur() const
{
  return dispersed().U() - continuous().U();
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::Re() const
{
  return magUr()*dispersed().d()/continuous().nu();
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::Pr() const
{
  return
    continuous().nu()*continuous().Cp()*continuous().rho()
    /continuous().kappa();
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::Eo() const
{
  return EoH(dispersed().d());
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::EoH1() const
{
  return EoH(dispersed().d()*cbrt(1 + 0.163*pow(Eo(), 0.757)));
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::EoH2() const
{
  return EoH(dispersed().d()/cbrt(E()));
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::Mo() const
{
  return mag(g())*continuous().nu()
    *pow3(continuous().nu()*continuous().rho()/sigma());
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::Ta() const
{
  return Re()*pow(Mo(), 0.23);
}
mousse::tmp<mousse::volScalarField> mousse::phasePair::E() const
{
  FATAL_ERROR_IN("mousse::phasePair::E() const")
    << "Requested aspect ratio of the dispersed phase in an unordered pair"
    << exit(FatalError);
  return phase1();
}
