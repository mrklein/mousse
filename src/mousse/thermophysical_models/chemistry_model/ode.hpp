#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_ODE_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_ODE_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ode
// Description
//   An ODE solver for chemistry

#include "chemistry_solver.hpp"
#include "ode_solver.hpp"


namespace mousse {

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
  TYPE_NAME("ode");
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

#include "ode.ipp"

#endif
