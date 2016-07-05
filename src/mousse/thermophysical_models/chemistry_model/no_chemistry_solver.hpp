#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_NO_CHEMISTY_SOLVER_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_NO_CHEMISTY_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noChemistrySolver
// Description
//   Dummy chemistry solver for 'none' option

#include "chemistry_solver.hpp"


namespace mousse {

template<class ChemistryModel>
class noChemistrySolver
:
  public chemistrySolver<ChemistryModel>
{
public:
  //- Runtime type information
  TYPE_NAME("none");
  // Constructors
    //- Construct from mesh and phase name
    noChemistrySolver(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~noChemistrySolver();
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

#include "no_chemistry_solver.ipp"

#endif
