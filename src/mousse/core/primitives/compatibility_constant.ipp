// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "compatibility_constant.hpp"


// Constructors 
template<class Type>
mousse::CompatibilityConstant<Type>::CompatibilityConstant
(
  const word& entryName,
  const dictionary& dict
)
:
  DataEntry<Type>{entryName},
  value_{pTraits<Type>::zero},
  dimensions_{dimless}
{
  Istream& is{dict.lookup(entryName)};
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
mousse::CompatibilityConstant<Type>::CompatibilityConstant
(
  const CompatibilityConstant<Type>& cnst
)
:
  DataEntry<Type>{cnst},
  value_{cnst.value_},
  dimensions_{cnst.dimensions_}
{}


// Destructor 
template<class Type>
mousse::CompatibilityConstant<Type>::~CompatibilityConstant()
{}


// Member Functions 
template<class Type>
Type mousse::CompatibilityConstant<Type>::value(const scalar) const
{
  return value_;
}


template<class Type>
Type mousse::CompatibilityConstant<Type>::integrate
(
  const scalar x1,
  const scalar x2
) const
{
  return (x2 - x1)*value_;
}


template<class Type>
mousse::dimensioned<Type> mousse::CompatibilityConstant<Type>::
dimValue(const scalar) const
{
  return dimensioned<Type>{"dimensionedValue", dimensions_, value_};
}


template<class Type>
mousse::dimensioned<Type> mousse::CompatibilityConstant<Type>::dimIntegrate
(
  const scalar x1, const scalar x2
) const
{
  return dimensioned<Type>("dimensionedValue", dimensions_, (x2-x1)*value_);
}


//  IOStream operators
#include "compatibility_constant_io.ipp"
