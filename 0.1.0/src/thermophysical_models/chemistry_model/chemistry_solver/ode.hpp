// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ode
// Description
//   An ODE solver for chemistry
// SourceFiles
//   ode.cpp
#ifndef ode_hpp_
#define ode_hpp_
#include "chemistry_solver.hpp"
#include "ode_solver.hpp"
namespace mousse
{
template<class ChemistryModel>
class ode
:
  public chemistrySolver<ChemistryModel>
{
  // Private data
    dictionary coeffsDict_;
    autoPtr<ODESolver> odeSolver_;
    // Solver data
    mutable scalarField cTp_;
public:
  //- Runtime type information
  TypeName("ode");
  // Constructors
    //- Construct from mesh and phase name
    ode(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~ode();
  // Member Functions
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
#   include "ode.cpp"
#endif
#endif
