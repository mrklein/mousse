// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "polynomial_entry.hpp"


// Constructors 
template<class Type>
mousse::PolynomialEntry<Type>::PolynomialEntry
(
  const word& entryName,
  const dictionary& dict
)
:
  DataEntry<Type>{entryName},
  coeffs_{},
  canIntegrate_{true},
  dimensions_{dimless}
{
  Istream& is{dict.lookup(entryName)};
  word entryType{is};
  token firstToken{is};
  is.putBack(firstToken);
  if (firstToken == token::BEGIN_SQR)
  {
    is >> this->dimensions_;
  }
  is >> coeffs_;
  if (!coeffs_.size())
  {
    FATAL_ERROR_IN
    (
      "PolynomialEntry<Type>::"
      "PolynomialEntry(const word&, const dictionary&)"
    )
    << "PolynomialEntry coefficients for entry " << this->name_
    << " are invalid (empty)" << nl << exit(FatalError);
  }
  FOR_ALL(coeffs_, i)
  {
    if (mag(coeffs_[i].second() + pTraits<Type>::one) < ROOTVSMALL)
    {
      canIntegrate_ = false;
      break;
    }
  }
  if (debug)
  {
    if (!canIntegrate_)
    {
      WARNING_IN
      (
        "PolynomialEntry<Type>::PolynomialEntry"
        "(const word&, const dictionary&)"
      )
      << "PolynomialEntry " << this->name_ << " cannot be integrated"
      << endl;
    }
  }
}


template<class Type>
mousse::PolynomialEntry<Type>::PolynomialEntry
(
  const word& entryName,
  const List<Tuple2<Type, Type>>& coeffs
)
:
  DataEntry<Type>{entryName},
  coeffs_{coeffs},
  canIntegrate_{true},
  dimensions_{dimless}
{
  if (!coeffs_.size())
  {
    FATAL_ERROR_IN
    (
      "mousse::PolynomialEntry<Type>::PolynomialEntry"
      "(const word&, const List<Tuple2<Type, Type>>&)"
    )
    << "PolynomialEntry coefficients for entry " << this->name_
    << " are invalid (empty)" << nl << exit(FatalError);
  }
  FOR_ALL(coeffs_, i)
  {
    if (mag(coeffs_[i].second() + 1) < ROOTVSMALL)
    {
      canIntegrate_ = false;
      break;
    }
  }
  if (debug)
  {
    if (!canIntegrate_)
    {
      WARNING_IN
      (
        "mousse::PolynomialEntry<Type>::PolynomialEntry"
        "(const word&, const List<Tuple2<Type, Type>>&)"
      )
      << "PolynomialEntry " << this->name_ << " cannot be integrated"
      << endl;
    }
  }
}


template<class Type>
mousse::PolynomialEntry<Type>::PolynomialEntry(const PolynomialEntry& poly)
:
  DataEntry<Type>{poly},
  coeffs_{poly.coeffs_},
  canIntegrate_{poly.canIntegrate_},
  dimensions_{poly.dimensions_}
{}


// Destructor 
template<class Type>
mousse::PolynomialEntry<Type>::~PolynomialEntry()
{}


// Member Functions 
template<class Type>
void mousse::PolynomialEntry<Type>::convertTimeBase(const Time& t)
{
  FOR_ALL(coeffs_, i)
  {
    Type value = coeffs_[i].first();
    for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
      setComponent(coeffs_[i].first(), cmpt) =
        t.userTimeToTime(component(value, cmpt));
    }
  }
}


template<class Type>
Type mousse::PolynomialEntry<Type>::value(const scalar x) const
{
  Type y{pTraits<Type>::zero};
  FOR_ALL(coeffs_, i)
  {
    y += cmptMultiply
    (
      coeffs_[i].first(),
      cmptPow(pTraits<Type>::one*x, coeffs_[i].second())
    );
  }
  return y;
}


template<class Type>
Type mousse::PolynomialEntry<Type>::integrate
(
  const scalar x1,
  const scalar x2
) const
{
  Type intx{pTraits<Type>::zero};
  if (canIntegrate_)
  {
    FOR_ALL(coeffs_, i)
    {
      intx += cmptMultiply
      (
        cmptDivide
        (
          coeffs_[i].first(),
          coeffs_[i].second() + pTraits<Type>::one
        ),
        cmptPow
        (
          pTraits<Type>::one*x2,
          coeffs_[i].second() + pTraits<Type>::one
        )
       - cmptPow
        (
          pTraits<Type>::one*x1,
          coeffs_[i].second() + pTraits<Type>::one
        )
      );
    }
  }
  return intx;
}


template<class Type>
mousse::dimensioned<Type> mousse::PolynomialEntry<Type>::dimValue
(
  const scalar x
) const
{
  return dimensioned<Type>{"dimensionedValue", dimensions_, value(x)};
}


template<class Type>
mousse::dimensioned<Type> mousse::PolynomialEntry<Type>::dimIntegrate
(
  const scalar x1,
  const scalar x2
) const
{
  return dimensioned<Type>
  {
    "dimensionedValue",
    dimensions_,
    integrate(x1, x2)
  };
}

//  IOStream operators
#include "polynomial_entry_io.ipp"
