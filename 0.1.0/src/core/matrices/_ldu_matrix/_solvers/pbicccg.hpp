// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PBiCCCG
// Description
//   Preconditioned bi-conjugate gradient solver for asymmetric lduMatrices
//   using a run-time selectable preconditiioner.
// SourceFiles
//   pbicccg.cpp

#ifndef pbicccg_hpp_
#define pbicccg_hpp_

#include "_ldu_matrix.hpp"

namespace mousse
{
template<class Type, class DType, class LUType>
class PBiCCCG
:
  public LduMatrix<Type, DType, LUType>::solver
{
public:

  //- Runtime type information
  TYPE_NAME("PBiCCCG");

  // Constructors

    //- Construct from matrix components and solver data dictionary
    PBiCCCG
    (
      const word& fieldName,
      const LduMatrix<Type, DType, LUType>& matrix,
      const dictionary& solverDict
    );

    //- Disallow default bitwise copy construct
    PBiCCCG(const PBiCCCG&) = delete;

    //- Disallow default bitwise assignment
    PBiCCCG& operator=(const PBiCCCG&) = delete;

  // Destructor
    virtual ~PBiCCCG()
    {}

  // Member Functions

    //- Solve the matrix with this solver
    virtual SolverPerformance<Type> solve(Field<Type>& psi) const;

};

}  // namespace mousse
#ifdef NoRepository
#   include "pbicccg.cpp"
#endif
#endif
