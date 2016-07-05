#ifndef THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_CHEMISTRY_SOLVER_HPP_
#define THERMOPHYSICAL_MODELS_CHEMISTRY_MODEL_CHEMISTRY_SOLVER_CHEMISTRY_SOLVER_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemistrySolver
// Description
//   An abstract base class for solving chemistry

#include "chemistry_model.hpp"
#include "iodictionary.hpp"
#include "scalar_field.hpp"


namespace mousse {

template<class ChemistryModel>
class chemistrySolver
:
  public ChemistryModel
{
public:
  // Constructors
    //- Construct from components and phase name
    chemistrySolver(const fvMesh& mesh, const word& phaseName);
  //- Destructor
  virtual ~chemistrySolver();
  // Member Functions
    //- Update the concentrations and return the chemical time
    virtual void solve
    (
      scalarField &c,
      scalar& T,
      scalar& p,
      scalar& deltaT,
      scalar& subDeltaT
    ) const = 0;
};

}  // namespace mousse

#include "chemistry_solver.ipp"

#endif
