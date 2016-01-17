// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::noChemistrySolver
// Description
//   Dummy chemistry solver for 'none' option
// SourceFiles
//   no_chemistry_solver.hpp
//   no_chemistry_solver.cpp
#ifndef noChemistySolver_H
#define noChemistySolver_H
#include "chemistry_solver.hpp"
namespace mousse
{
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
#ifdef NoRepository
#   include "no_chemistry_solver.cpp"
#endif
#endif
