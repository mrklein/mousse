// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "_integration_scheme.hpp"


// Constructors 
template<class Type>
mousse::IntegrationScheme<Type>::IntegrationScheme
(
  const word& phiName,
  const dictionary& dict
)
:
 phiName_{phiName},
 dict_{dict}
{}


template<class Type>
mousse::IntegrationScheme<Type>::IntegrationScheme(const IntegrationScheme& is)
:
  phiName_{is.phiName_},
  dict_{is.dict_}
{}


// Destructor   
template<class Type>
mousse::IntegrationScheme<Type>::~IntegrationScheme()
{}


#include "_integration_scheme_new.ipp"

