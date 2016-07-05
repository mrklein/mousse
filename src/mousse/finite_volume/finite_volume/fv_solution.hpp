#ifndef FINITE_VOLUME_FINITE_VOLUME_FV_SOLUTION_HPP_
#define FINITE_VOLUME_FINITE_VOLUME_FV_SOLUTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvSolution
// Description
//   Selector class for finite volume solution solution.
//   fvMesh is derived from fvSolution so that all fields have access to the
//   fvSolution from the mesh reference they hold.

#include "solution.hpp"


namespace mousse {

class fvSolution
:
  public solution
{
public:
  // Constructors
    //- Construct for objectRegistry
    fvSolution(const objectRegistry& obr)
    :
      solution{obr, "fvSolution"}
    {}
    //- Disallow default bitwise copy construct and assignment
    fvSolution(const fvSolution&) = delete;
    fvSolution& operator=(const fvSolution&) = delete;
};

}  // namespace mousse

#endif
