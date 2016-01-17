// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::SolverPerformance
// Description
//   SolverPerformance is the class returned by the LduMatrix solver
//   containing performance statistics.
// SourceFiles
//   _solver_performance.cpp

#ifndef _solver_performance_hpp_
#define _solver_performance_hpp_

#include "word.hpp"
#include "fixed_list.hpp"
#include "class_name.hpp"

namespace mousse
{
// Forward declaration of friend functions and operators
template<class Type>
class SolverPerformance;
template<class Type>
SolverPerformance<Type> max
(
  const SolverPerformance<Type>&,
  const SolverPerformance<Type>&
);
template<class Type>
Istream& operator>>
(
  Istream&,
  SolverPerformance<Type>&
);
template<class Type>
Ostream& operator<<
(
  Ostream&,
  const SolverPerformance<Type>&
);
template<class Type>
class SolverPerformance
{
  word   solverName_;
  word   fieldName_;
  Type   initialResidual_;
  Type   finalResidual_;
  label  noIterations_;
  bool   converged_;
  FixedList<bool, pTraits<Type>::nComponents> singular_;
public:
  // Static data
    // Declare name of the class and its debug switch
    CLASS_NAME("SolverPerformance");
    //- Large Type for the use in solvers
    static const scalar great_;
    //- Small Type for the use in solvers
    static const scalar small_;
    //- Very small Type for the use in solvers
    static const scalar vsmall_;
  // Constructors
    SolverPerformance()
    :
      initialResidual_(pTraits<Type>::zero),
      finalResidual_(pTraits<Type>::zero),
      noIterations_(0),
      converged_(false),
      singular_(false)
    {}
    SolverPerformance
    (
      const word&  solverName,
      const word&  fieldName,
      const Type&  iRes = pTraits<Type>::zero,
      const Type&  fRes = pTraits<Type>::zero,
      const label  nIter = 0,
      const bool   converged = false,
      const bool   singular = false
    )
    :
      solverName_(solverName),
      fieldName_(fieldName),
      initialResidual_(iRes),
      finalResidual_(fRes),
      noIterations_(nIter),
      converged_(converged),
      singular_(singular)
    {}
  // Member functions
    //- Return solver name
    const word& solverName() const
    {
      return solverName_;
    }
    //- Return solver name
    word& solverName()
    {
      return solverName_;
    }
    //- Return field name
    const word& fieldName() const
    {
      return fieldName_;
    }
    //- Return initial residual
    const Type& initialResidual() const
    {
      return initialResidual_;
    }
    //- Return initial residual
    Type& initialResidual()
    {
      return initialResidual_;
    }
    //- Return final residual
    const Type& finalResidual() const
    {
      return finalResidual_;
    }
    //- Return final residual
    Type& finalResidual()
    {
      return finalResidual_;
    }
    //- Return number of iterations
    label nIterations() const
    {
      return noIterations_;
    }
    //- Return number of iterations
    label& nIterations()
    {
      return noIterations_;
    }
    //- Has the solver converged?
    bool converged() const
    {
      return converged_;
    }
    //- Is the matrix singular?
    bool singular() const;
    //- Check, store and return convergence
    bool checkConvergence
    (
      const Type& tolerance,
      const Type& relTolerance
    );
    //- Singularity test
    bool checkSingularity(const Type& residual);
    //- Print summary of solver performance to the given stream
    void print(Ostream& os) const;
  // Member Operators
    bool operator!=(const SolverPerformance<Type>&) const;
  // Friend functions
    //- Return the element-wise maximum of two SolverPerformance<Type>s
    friend SolverPerformance<Type> max <Type>
    (
      const SolverPerformance<Type>&,
      const SolverPerformance<Type>&
    );
  // Ostream Operator
    friend Istream& operator>> <Type>
    (
      Istream&,
      SolverPerformance<Type>&
    );
    friend Ostream& operator<< <Type>
    (
      Ostream&,
      const SolverPerformance<Type>&
    );
};
}  // namespace mousse
#define MAKE_SOLVER_PERFORMANCE(Type)                                         \
                                                                              \
typedef mousse::SolverPerformance<Type>                                       \
  solverPerformance##Type;                                                    \
                                                                              \
DEFINE_NAMED_TEMPLATE_TYPE_NAME_AND_DEBUG(solverPerformance##Type, 0);        \
                                                                              \
template<>                                                                    \
const scalar solverPerformance##Type::great_(1e20);                           \
                                                                              \
template<>                                                                    \
const scalar solverPerformance##Type::small_(1e-20);                          \
                                                                              \
template<>                                                                    \
const scalar solverPerformance##Type::vsmall_(VSMALL);                        \

#ifdef NoRepository
#   include "_solver_performance.cpp"
#endif
#endif
