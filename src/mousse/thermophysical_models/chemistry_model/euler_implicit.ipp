// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "euler_implicit.hpp"
#include "add_to_run_time_selection_table.hpp"
#include "simple_matrix.hpp"


// Constructors 
template<class ChemistryModel>
mousse::EulerImplicit<ChemistryModel>::EulerImplicit
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  chemistrySolver<ChemistryModel>{mesh, phaseName},
  coeffsDict_{this->subDict("EulerImplicitCoeffs")},
  cTauChem_{readScalar(coeffsDict_.lookup("cTauChem"))},
  eqRateLimiter_{coeffsDict_.lookup("equilibriumRateLimiter")},
  cTp_{this->nEqns()}
{}


// Destructor 
template<class ChemistryModel>
mousse::EulerImplicit<ChemistryModel>::~EulerImplicit()
{}


// Member Functions 
template<class ChemistryModel>
void mousse::EulerImplicit<ChemistryModel>::updateRRInReactionI
(
  const label index,
  const scalar pr,
  const scalar pf,
  const scalar corr,
  const label lRef,
  const label rRef,
  const scalar /*p*/,
  const scalar /*T*/,
  simpleMatrix<scalar>& RR
) const
{
  const Reaction<typename ChemistryModel::thermoType>& R =
    this->reactions_[index];
  FOR_ALL(R.lhs(), s) {
    label si = R.lhs()[s].index;
    scalar sl = R.lhs()[s].stoichCoeff;
    RR[si][rRef] -= sl*pr*corr;
    RR[si][lRef] += sl*pf*corr;
  }
  FOR_ALL(R.rhs(), s) {
    label si = R.rhs()[s].index;
    scalar sr = R.rhs()[s].stoichCoeff;
    RR[si][lRef] -= sr*pf*corr;
    RR[si][rRef] += sr*pr*corr;
  }
}


template<class ChemistryModel>
void mousse::EulerImplicit<ChemistryModel>::solve
(
  scalarField& c,
  scalar& T,
  scalar& p,
  scalar& deltaT,
  scalar& subDeltaT
) const
{
  const label nSpecie = this->nSpecie();
  simpleMatrix<scalar> RR{nSpecie, 0, 0};
  for (label i=0; i<nSpecie; i++) {
    c[i] = max(0.0, c[i]);
  }
  // Calculate the absolute enthalpy
  scalar cTot = sum(c);
  typename ChemistryModel::thermoType
    mixture((c[0]/cTot)*this->specieThermo_[0]);
  for (label i=1; i<nSpecie; i++) {
    mixture += (c[i]/cTot)*this->specieThermo_[i];
  }
  scalar ha = mixture.Ha(p, T);
  scalar deltaTEst = min(deltaT, subDeltaT);
  FOR_ALL(this->reactions(), i) {
    scalar pf, cf, pr, cr;
    label lRef, rRef;
    scalar omegai = this->omegaI(i, c, T, p, pf, cf, lRef, pr, cr, rRef);
    scalar corr = 1.0;
    if (eqRateLimiter_) {
      if (omegai < 0.0) {
        corr = 1.0/(1.0 + pr*deltaTEst);
      } else {
        corr = 1.0/(1.0 + pf*deltaTEst);
      }
    }
    updateRRInReactionI(i, pr, pf, corr, lRef, rRef, p, T, RR);
  }
  // Calculate the stable/accurate time-step
  scalar tMin = GREAT;
  for (label i=0; i<nSpecie; i++) {
    scalar d = 0;
    for (label j=0; j<nSpecie; j++) {
      d -= RR[i][j]*c[j];
    }
    if (d < -SMALL) {
      tMin = min(tMin, -(c[i] + SMALL)/d);
    } else {
      d = max(d, SMALL);
      scalar cm = max(cTot - c[i], 1.0e-5);
      tMin = min(tMin, cm/d);
    }
  }
  subDeltaT = cTauChem_*tMin;
  deltaT = min(deltaT, subDeltaT);
  // Add the diagonal and source contributions from the time-derivative
  for (label i=0; i<nSpecie; i++) {
    RR[i][i] += 1.0/deltaT;
    RR.source()[i] = c[i]/deltaT;
  }
  // Solve for the new composition
  c = RR.LUsolve();
  // Limit the composition
  for (label i=0; i<nSpecie; i++) {
    c[i] = max(0.0, c[i]);
  }
  // Update the temperature
  cTot = sum(c);
  mixture = (c[0]/cTot)*this->specieThermo_[0];
  for (label i=1; i<nSpecie; i++) {
    mixture += (c[i]/cTot)*this->specieThermo_[i];
  }
  T = mixture.THa(ha, p, T);
}

