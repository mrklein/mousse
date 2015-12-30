// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fvSolution
// Description
//   Selector class for finite volume solution solution.
//   fvMesh is derived from fvSolution so that all fields have access to the
//   fvSolution from the mesh reference they hold.
#ifndef fv_solution_hpp_
#define fv_solution_hpp_
#include "solution.hpp"
namespace mousse
{
class fvSolution
:
  public solution
{
  // Private Member Functions
    //- Disallow default bitwise copy construct and assignment
    fvSolution(const fvSolution&);
    void operator=(const fvSolution&);
public:
  // Constructors
    //- Construct for objectRegistry
    fvSolution(const objectRegistry& obr)
    :
      solution(obr, "fvSolution")
    {}
};
}  // namespace mousse
#endif
