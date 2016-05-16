// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_euler.hpp"


// Constructors 
template<class Type>
mousse::Euler<Type>::Euler
(
  const word& phiName,
  const dictionary& dict
)
:
  IntegrationScheme<Type>{phiName, dict}
{}


template<class Type>
mousse::Euler<Type>::Euler(const Euler& is)
:
  IntegrationScheme<Type>{is}
{}


// Destructor 
template<class Type>
mousse::Euler<Type>::~Euler()
{}


// Member Functions 
template<class Type>
typename mousse::IntegrationScheme<Type>::integrationResult
mousse::Euler<Type>::integrate
(
  const Type& phi,
  const scalar dt,
  const Type& alphaBeta,
  const scalar beta
) const
{
  typename IntegrationScheme<Type>::integrationResult retValue;
  retValue.value() = (phi + alphaBeta*dt)/(1.0 + beta*dt);
  retValue.average() = 0.5*(phi + retValue.value());
  return retValue;
}

