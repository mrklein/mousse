#ifndef CORE_PRIMITIVES_FUNCTIONS_POLYNOMIAL_HPP_
#define CORE_PRIMITIVES_FUNCTIONS_POLYNOMIAL_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Polynomial
// Description
//   Polynomial templated on size (order):
//     poly = sum(coeff_[i]*x^i) logCoeff*log(x)
//   where 0 \<= i \<= N
//   - integer powers, starting at zero
//   - value(x) to evaluate the poly for a given value
//   - derivative(x) returns derivative at value
//   - integral(x1, x2) returns integral between two scalar values
//   - integral() to return a new, integral coeff polynomial
//    - increases the size (order)
//   - integralMinus1() to return a new, integral coeff polynomial where
//    the base poly starts at order -1

#include "word.hpp"
#include "scalar.hpp"
#include "ostream.hpp"
#include "vector_space.hpp"
#include "static_assert.hpp"


namespace mousse {

// Forward declaration of classes
template<int PolySize>
class Polynomial;

// Forward declaration of friend functions
template<int PolySize>
Ostream& operator<<
(
  Ostream&,
  const Polynomial<PolySize>&
);


template<int PolySize>
class Polynomial
:
  public VectorSpace<Polynomial<PolySize>, scalar, PolySize>
{
  //- Size must be positive (non-zero)
  STATIC_ASSERT(PolySize > 0);
  // Private data
    //- Include the log term? - only activated using integralMinus1()
    bool logActive_;
    //- Log coefficient - only activated using integralMinus1()
    scalar logCoeff_;
public:
  typedef Polynomial<PolySize> polyType;
  typedef Polynomial<PolySize + 1> intPolyType;
  // Constructors
    //- Construct null, with all coefficients = 0.0
    Polynomial();
    //- Copy constructor
    Polynomial(const Polynomial&);
    //- Construct from C-array of coefficients
    explicit Polynomial(const scalar coeffs[PolySize]);
    //- Construct from a list of coefficients
    explicit Polynomial(const UList<scalar>& coeffs);
    //- Construct from Istream
    Polynomial(Istream&);
    //- Construct from name and Istream
    Polynomial(const word& name, Istream&);
  // Member Functions
    // Access
      //- Return true if the log term is active
      bool logActive() const;
      //- Return the log coefficient
      scalar logCoeff() const;
    // Evaluation
      //- Return polynomial value
      scalar value(const scalar x) const;
      //- Return derivative of the polynomial at the given x
      scalar derivative(const scalar x) const;
      //- Return integral between two values
      scalar integral(const scalar x1, const scalar x2) const;
      //- Return integral coefficients.
      //  Argument becomes zero'th element (constant of integration)
      intPolyType integral(const scalar intConstant = 0.0) const;
      //- Return integral coefficients when lowest order is -1.
      //  Argument becomes zero'th element (constant of integration)
      polyType integralMinus1(const scalar intConstant = 0.0) const;
  //- Ostream Operator
  friend Ostream& operator<< <PolySize>
  (
    Ostream&,
    const Polynomial&
  );
};
}  // namespace mousse

#include "polynomial.ipp"
#include "polynomial_io.ipp"

#endif
