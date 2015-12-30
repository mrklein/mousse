// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::chemistrySolver
// Description
//   An abstract base class for solving chemistry
// SourceFiles
//   chemistry_solver.cpp
#ifndef chemistry_solver_hpp_
#define chemistry_solver_hpp_
#include "chemistry_model.hpp"
#include "iodictionary.hpp"
#include "scalar_field.hpp"
namespace mousse
{
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
#ifdef NoRepository
#   include "chemistry_solver.cpp"
#endif
#endif
