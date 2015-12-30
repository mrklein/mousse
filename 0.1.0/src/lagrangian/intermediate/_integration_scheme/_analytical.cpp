// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_analytical.hpp"
// Constructors 
template<class Type>
mousse::Analytical<Type>::Analytical
(
  const word& phiName,
  const dictionary& dict
)
:
  IntegrationScheme<Type>(phiName, dict)
{}
template<class Type>
mousse::Analytical<Type>::Analytical(const Analytical& is)
:
  IntegrationScheme<Type>(is)
{}
// Destructor 
template<class Type>
mousse::Analytical<Type>::~Analytical()
{}
// Member Functions 
template<class Type>
typename mousse::IntegrationScheme<Type>::integrationResult
mousse::Analytical<Type>::integrate
(
  const Type& phi,
  const scalar dt,
  const Type& alphaBeta,
  const scalar beta
) const
{
  typename IntegrationScheme<Type>::integrationResult retValue;
  const scalar expTerm = exp(min(50, -beta*dt));
  if (beta > ROOTVSMALL)
  {
    const Type alpha = alphaBeta/beta;
    retValue.average() = alpha + (phi - alpha)*(1 - expTerm)/(beta*dt);
    retValue.value() =  alpha + (phi - alpha)*expTerm;
  }
  else
  {
    retValue.average() = phi;
    retValue.value() =  phi;
  }
  return retValue;
}
