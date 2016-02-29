#ifndef CORE_PRIMITIVES_FUNCTIONS_POLYNOMIAL_FUNCTION_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_POLYNOMIAL_FUNCTION_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::polynomialFunction
// Description
//   Polynomial function representation
//     poly = logCoeff*log(x) + sum(coeff_[i]*x^i)
//   where 0 \<= i \<= N
//   - integer powers, starting at zero
//   - value(x) to evaluate the poly for a given value
//   - integrate(x1, x2) between two scalar values
//   - integral() to return a new, integral coeff polynomial
//    - increases the size (order)
//   - integralMinus1() to return a new, integral coeff polynomial where
//    the base poly starts at order -1
// SeeAlso
//   mousse::Polynomial for a templated implementation
// SourceFiles
//   polynomial_function.cpp
#include "scalar_list.hpp"
#include "ostream.hpp"
#include "run_time_selection_tables.hpp"
namespace mousse
{
// Forward declaration of classes
class polynomialFunction;
// Forward declaration of friend functions
Ostream& operator<<(Ostream&, const polynomialFunction&);
class polynomialFunction
:
  private scalarList
{
  // Private data
    //- Include the log term? - only activated using integralMinus1()
    bool logActive_;
    //- Log coefficient - only activated using integralMinus1()
    scalar logCoeff_;
  // Private Member Functions
    //- Return integral coefficients.
    //  Argument becomes zeroth element (constant of integration)
    static polynomialFunction cloneIntegral
    (
      const polynomialFunction&,
      const scalar intConstant = 0.0
    );
    //- Return integral coefficients when lowest order is -1.
    //  Argument becomes zeroth element (constant of integration)
    static polynomialFunction cloneIntegralMinus1
    (
      const polynomialFunction&,
      const scalar intConstant = 0.0
    );
public:
  //- Runtime type information
  TYPE_NAME("polynomialFunction");
  // Constructors
    //- Construct a particular size, with all coefficients = 0.0
    explicit polynomialFunction(const label);
    //- Copy constructor
    polynomialFunction(const polynomialFunction&);
    //- Construct from a list of coefficients
    explicit polynomialFunction(const UList<scalar>& coeffs);
    //- Construct from Istream
    polynomialFunction(Istream&);
    //- Disallow default bitwise assignment
    polynomialFunction& operator=(const polynomialFunction&) = delete;
  //- Destructor
  virtual ~polynomialFunction();
  // Member Functions
      //- Return the number of coefficients
      using scalarList::size;
      //- Return coefficient
      using scalarList::operator[];
    // Access
      //- Return true if the log term is active
      bool logActive() const;
      //- Return the log coefficient
      scalar logCoeff() const;
    // Evaluation
      //- Return polynomial value
      scalar value(const scalar x) const;
      //- Integrate between two values
      scalar integrate(const scalar x1, const scalar x2) const;
      //- Return integral coefficients.
      //  Argument becomes zeroth element (constant of integration)
      polynomialFunction integral
      (
        const scalar intConstant = 0.0
      ) const;
      //- Return integral coefficients when lowest order is -1.
      //  Argument becomes zeroth element (constant of integration)
      polynomialFunction integralMinus1
      (
        const scalar intConstant = 0.0
      ) const;
  // Member Operators
    polynomialFunction& operator+=(const polynomialFunction&);
    polynomialFunction& operator-=(const polynomialFunction&);
    polynomialFunction& operator*=(const scalar);
    polynomialFunction& operator/=(const scalar);
  //- Ostream Operator
  friend Ostream& operator<<(Ostream&, const polynomialFunction&);
};
// Global Operators 
polynomialFunction operator+
(
  const polynomialFunction&,
  const polynomialFunction&
);
polynomialFunction operator-
(
  const polynomialFunction&,
  const polynomialFunction&
);
polynomialFunction operator*
(
  const scalar,
  const polynomialFunction&
);
polynomialFunction operator/
(
  const scalar,
  const polynomialFunction&
);
polynomialFunction operator*
(
  const polynomialFunction&,
  const scalar
);
polynomialFunction operator/
(
  const polynomialFunction&,
  const scalar
);
}  // namespace mousse
#endif
