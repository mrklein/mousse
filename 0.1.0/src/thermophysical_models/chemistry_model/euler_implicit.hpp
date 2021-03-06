#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_EULER_IMPLICIT_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_EULER_IMPLICIT_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::EulerImplicit
// Description
//   An Euler implicit solver for chemistry

#include "chemistry_solver.hpp"


namespace mousse {

template<class ChemistryModel>
class EulerImplicit
:
  public chemistrySolver<ChemistryModel>
{
  // Private data
    //- Coefficients dictionary
    dictionary coeffsDict_;
    // Model constants
      //- Chemistry timescale
      scalar cTauChem_;
      //- Equilibrium rate limiter flag (on/off)
      Switch eqRateLimiter_;
    // Solver data
    mutable scalarField cTp_;
public:
  //- Runtime type information
  TYPE_NAME("EulerImplicit");
  // Constructors
    //- Construct from mesh and phase name
    EulerImplicit(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~EulerImplicit();
  // Member Functions
  void updateRRInReactionI
  (
    const label index,
    const scalar pr,
    const scalar pf,
    const scalar corr,
    const label lRef,
    const label rRef,
    const scalar p,
    const scalar T,
    simpleMatrix<scalar>& RR
  ) const;
    //- Update the concentrations and return the chemical time
    virtual void solve
    (
      scalarField& c,
      scalar& T,
      scalar& p,
      scalar& deltaT,
      scalar& subDeltaT
    ) const;
};

}  // namespace mousse

#include "euler_implicit.ipp"

#endif
