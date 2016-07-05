// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "ode.hpp"
#include "chemistry_model.hpp"


// Constructors 
template<class ChemistryModel>
mousse::ode<ChemistryModel>::ode
(
  const fvMesh& mesh,
  const word& phaseName
)
:
  chemistrySolver<ChemistryModel>{mesh, phaseName},
  coeffsDict_{this->subDict("odeCoeffs")},
  odeSolver_{ODESolver::New(*this, coeffsDict_)},
  cTp_{this->nEqns()}
{}


// Destructor 
template<class ChemistryModel>
mousse::ode<ChemistryModel>::~ode()
{}


// Member Functions 
template<class ChemistryModel>
void mousse::ode<ChemistryModel>::solve
(
  scalarField& c,
  scalar& T,
  scalar& p,
  scalar& deltaT,
  scalar& subDeltaT
) const
{
  label nSpecie = this->nSpecie();
  // Copy the concentration, T and P to the total solve-vector
  for (int i=0; i<nSpecie; i++) {
    cTp_[i] = c[i];
  }
  cTp_[nSpecie] = T;
  cTp_[nSpecie+1] = p;
  odeSolver_->solve(0, deltaT, cTp_, subDeltaT);
  for (int i=0; i<nSpecie; i++) {
    c[i] = max(0.0, cTp_[i]);
  }
  T = cTp_[nSpecie];
  p = cTp_[nSpecie+1];
}

