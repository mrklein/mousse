// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial_function.hpp"
#include "add_to_run_time_selection_table.hpp"


// Static Data Members
namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(polynomialFunction, 0);

}


// Static Member Functions
mousse::polynomialFunction mousse::polynomialFunction::cloneIntegral
(
  const polynomialFunction& poly,
  const scalar intConstant
)
{
  polynomialFunction newPoly{poly.size()+1};
  newPoly[0] = intConstant;
  FOR_ALL(poly, i)
  {
    newPoly[i+1] = poly[i]/(i + 1);
  }
  return newPoly;
}


mousse::polynomialFunction mousse::polynomialFunction::cloneIntegralMinus1
(
  const polynomialFunction& poly,
  const scalar intConstant
)
{
  polynomialFunction newPoly{poly.size()+1};
  if (poly[0] > VSMALL)
  {
    newPoly.logActive_ = true;
    newPoly.logCoeff_  = poly[0];
  }
  newPoly[0] = intConstant;
  for (label i=1; i < poly.size(); ++i)
  {
    newPoly[i] = poly[i]/i;
  }
  return newPoly;
}


// Constructors 
mousse::polynomialFunction::polynomialFunction(const label order)
:
  scalarList{order, 0.0},
  logActive_{false},
  logCoeff_{0.0}
{
  if (this->empty())
  {
    FATAL_ERROR_IN
    (
      "polynomialFunction::polynomialFunction(const label order)"
    )
    << "polynomialFunction coefficients are invalid (empty)"
    << nl << exit(FatalError);
  }
}


mousse::polynomialFunction::polynomialFunction(const polynomialFunction& poly)
:
  scalarList{poly},
  logActive_{poly.logActive_},
  logCoeff_{poly.logCoeff_}
{}


mousse::polynomialFunction::polynomialFunction(const UList<scalar>& coeffs)
:
  scalarList{coeffs},
  logActive_{false},
  logCoeff_{0.0}
{
  if (this->empty())
  {
    FATAL_ERROR_IN
    (
      "polynomialFunction::polynomialFunction(const UList<scalar>&)"
    )
    << "polynomialFunction coefficients are invalid (empty)"
    << nl << exit(FatalError);
  }
}


mousse::polynomialFunction::polynomialFunction(Istream& is)
:
  scalarList{is},
  logActive_{false},
  logCoeff_{0.0}
{
  if (this->empty())
  {
    FATAL_ERROR_IN
    (
      "polynomialFunction::polynomialFunction(Istream&)"
    )
    << "polynomialFunction coefficients are invalid (empty)"
    << nl << exit(FatalError);
  }
}


// Destructor 
mousse::polynomialFunction::~polynomialFunction()
{}


// Member Functions 
bool mousse::polynomialFunction::logActive() const
{
  return logActive_;
}


mousse::scalar mousse::polynomialFunction::logCoeff() const
{
  return logCoeff_;
}


mousse::scalar mousse::polynomialFunction::value(const scalar x) const
{
  const scalarList& coeffs = *this;
  scalar val = coeffs[0];
  // avoid costly pow() in calculation
  scalar powX = x;
  for (label i=1; i<coeffs.size(); ++i)
  {
    val += coeffs[i]*powX;
    powX *= x;
  }
  if (logActive_)
  {
    val += this->logCoeff_*log(x);
  }
  return val;
}


mousse::scalar mousse::polynomialFunction::integrate
(
  const scalar x1,
  const scalar x2
) const
{
  const scalarList& coeffs = *this;
  if (logActive_)
  {
    FATAL_ERROR_IN
    (
      "scalar polynomialFunction::integrate"
      "("
        "const scalar, "
        "const scalar"
      ") const"
    )
    << "Cannot integrate polynomial with logarithmic coefficients"
    << nl << abort(FatalError);
  }
  // avoid costly pow() in calculation
  scalar powX1 = x1;
  scalar powX2 = x2;
  scalar val = coeffs[0]*(powX2 - powX1);
  for (label i=1; i<coeffs.size(); ++i)
  {
    val += coeffs[i]/(i + 1)*(powX2 - powX1);
    powX1 *= x1;
    powX2 *= x2;
  }
  return val;
}


mousse::polynomialFunction
mousse::polynomialFunction::integral(const scalar intConstant) const
{
  return cloneIntegral(*this, intConstant);
}


mousse::polynomialFunction
mousse::polynomialFunction::integralMinus1(const scalar intConstant) const
{
  return cloneIntegralMinus1(*this, intConstant);
}


// Member Operators 
mousse::polynomialFunction&
mousse::polynomialFunction::operator+=(const polynomialFunction& poly)
{
  scalarList& coeffs = *this;
  if (coeffs.size() > poly.size())
  {
    FOR_ALL(poly, i)
    {
      coeffs[i] += poly[i];
    }
  }
  else
  {
    coeffs.setSize(poly.size(), 0.0);
    FOR_ALL(coeffs, i)
    {
      coeffs[i] += poly[i];
    }
  }
  return *this;
}


mousse::polynomialFunction&
mousse::polynomialFunction::operator-=(const polynomialFunction& poly)
{
  scalarList& coeffs = *this;
  if (coeffs.size() > poly.size())
  {
    FOR_ALL(poly, i)
    {
      coeffs[i] -= poly[i];
    }
  }
  else
  {
    coeffs.setSize(poly.size(), 0.0);
    FOR_ALL(coeffs, i)
    {
      coeffs[i] -= poly[i];
    }
  }
  return *this;
}


mousse::polynomialFunction&
mousse::polynomialFunction::operator*=(const scalar s)
{
  scalarList& coeffs = *this;
  FOR_ALL(coeffs, i)
  {
    coeffs[i] *= s;
  }
  return *this;
}


mousse::polynomialFunction&
mousse::polynomialFunction::operator/=(const scalar s)
{
  scalarList& coeffs = *this;
  FOR_ALL(coeffs, i)
  {
    coeffs[i] /= s;
  }
  return *this;
}


// IOstream Operators 
mousse::Ostream& mousse::operator<<(Ostream& os, const polynomialFunction& poly)
{
  // output like VectorSpace
  os << token::BEGIN_LIST;
  if (!poly.empty())
  {
    for (int i=0; i<poly.size()-1; i++)
    {
      os << poly[i] << token::SPACE;
    }
    os << poly.last();
  }
  os << token::END_LIST;
  // Check state of Ostream
  os.check("operator<<(Ostream&, const polynomialFunction&)");
  return os;
}


// Global Operators 
mousse::polynomialFunction
mousse::operator+
(
  const polynomialFunction& p1,
  const polynomialFunction& p2
)
{
  polynomialFunction poly{p1};
  return poly += p2;
}


mousse::polynomialFunction
mousse::operator-
(
  const polynomialFunction& p1,
  const polynomialFunction& p2
)
{
  polynomialFunction poly{p1};
  return poly -= p2;
}


mousse::polynomialFunction
mousse::operator*
(
  const scalar s,
  const polynomialFunction& p
)
{
  polynomialFunction poly{p};
  return poly *= s;
}


mousse::polynomialFunction
mousse::operator/
(
  const scalar s,
  const polynomialFunction& p
)
{
  polynomialFunction poly{p};
  return poly /= s;
}


mousse::polynomialFunction
mousse::operator*
(
  const polynomialFunction& p,
  const scalar s
)
{
  polynomialFunction poly{p};
  return poly *= s;
}


mousse::polynomialFunction
mousse::operator/
(
  const polynomialFunction& p,
  const scalar s
)
{
  polynomialFunction poly{p};
  return poly /= s;
}

