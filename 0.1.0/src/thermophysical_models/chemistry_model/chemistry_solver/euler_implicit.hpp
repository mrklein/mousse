// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::EulerImplicit
// Description
//   An Euler implicit solver for chemistry
// SourceFiles
//   euler_implicit.cpp
#ifndef euler_implicit_hpp_
#define euler_implicit_hpp_
#include "chemistry_solver.hpp"
namespace mousse
{
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
  TypeName("EulerImplicit");
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
#ifdef NoRepository
#   include "euler_implicit.cpp"
#endif
#endif
