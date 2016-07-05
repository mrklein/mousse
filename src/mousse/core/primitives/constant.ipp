// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "constant.hpp"


// Constructors 
template<class Type>
mousse::Constant<Type>::Constant(const word& entryName, const dictionary& dict)
:
  DataEntry<Type>{entryName},
  value_{pTraits<Type>::zero},
  dimensions_{dimless}
{
  Istream& is{dict.lookup(entryName)};
  word entryType{is};
  token firstToken{is};

  if (firstToken.isWord()) {
    token nextToken{is};

    if (nextToken == token::BEGIN_SQR) {
      is.putBack(nextToken);
      is >> dimensions_;
      is >> value_;
    }
  } else {
    is.putBack(firstToken);
    is >> value_;
  }
}


template<class Type>
mousse::Constant<Type>::Constant(const Constant<Type>& cnst)
:
  DataEntry<Type>{cnst},
  value_{cnst.value_},
  dimensions_{cnst.dimensions_}
{}


// Destructor 
template<class Type>
mousse::Constant<Type>::~Constant()
{}


// Member Functions 
template<class Type>
Type mousse::Constant<Type>::value(const scalar) const
{
  return value_;
}


template<class Type>
Type mousse::Constant<Type>::integrate(const scalar x1, const scalar x2) const
{
  return (x2 - x1)*value_;
}


template<class Type>
mousse::dimensioned<Type> mousse::Constant<Type>::dimValue(const scalar) const
{
  return dimensioned<Type>("dimensionedValue", dimensions_, value_);
}


template<class Type>
mousse::dimensioned<Type> mousse::Constant<Type>::dimIntegrate
(
  const scalar x1, const scalar x2
) const
{
  return dimensioned<Type>("dimensionedValue", dimensions_, (x2-x1)*value_);
}

//  IOStream operators
#include "constant_io.ipp"
