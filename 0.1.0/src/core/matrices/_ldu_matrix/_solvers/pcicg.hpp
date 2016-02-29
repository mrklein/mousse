#ifndef CORE_MATRICES_TLDU_MATRIX_TSOLVERS_PCICG_HPP_
#define CORE_MATRICES_TLDU_MATRIX_TSOLVERS_PCICG_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::PCICG
// Description
//   Preconditioned conjugate gradient solver for symmetric lduMatrices
//   using a run-time selectable preconditiioner.
// SourceFiles
//   pcicg.cpp


#include "_ldu_matrix.hpp"

namespace mousse
{
template<class Type, class DType, class LUType>
class PCICG
:
  public LduMatrix<Type, DType, LUType>::solver
{
public:

  //- Runtime type information
  TYPE_NAME("PCICG");

  // Constructors

    //- Construct from matrix components and solver data dictionary
    PCICG
    (
      const word& fieldName,
      const LduMatrix<Type, DType, LUType>& matrix,
      const dictionary& solverDict
    );

    //- Disallow default bitwise copy construct
    PCICG(const PCICG&) = delete;

    //- Disallow default bitwise assignment
    PCICG& operator=(const PCICG&) = delete;

  // Destructor
    virtual ~PCICG()
    {}

  // Member Functions

    //- Solve the matrix with this solver
    virtual SolverPerformance<Type> solve(Field<Type>& psi) const;
};

}  // namespace mousse

#ifdef NoRepository
#   include "pcicg.cpp"
#endif
#endif
