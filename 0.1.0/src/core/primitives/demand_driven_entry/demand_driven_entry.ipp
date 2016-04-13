// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "demand_driven_entry.hpp"


// Constructor
template<class Type>
mousse::demandDrivenEntry<Type>::demandDrivenEntry
(
  const dictionary& dict,
  const Type& value
)
:
  dict_{dict},
  keyword_{"unknown-keyword"},
  value_{value},
  stored_{true}
{}


template<class Type>
mousse::demandDrivenEntry<Type>::demandDrivenEntry
(
  const dictionary& dict,
  const word& keyword
)
:
  dict_{dict},
  keyword_{keyword},
  value_{pTraits<Type>::zero},
  stored_{false}
{}


template<class Type>
mousse::demandDrivenEntry<Type>::demandDrivenEntry
(
  const dictionary& dict,
  const word& keyword,
  const Type& defaultValue,
  const bool readIfPresent
)
:
  dict_{dict},
  keyword_{keyword},
  value_{defaultValue},
  stored_{true}
{
  if (readIfPresent) {
    dict_.readIfPresent<Type>(keyword, value_);
  }
}


template<class Type>
mousse::demandDrivenEntry<Type>::demandDrivenEntry(const demandDrivenEntry& dde)
:
  dict_{dde.dict_},
  keyword_{dde.keyword_},
  value_{dde.value_},
  stored_{dde.stored_}
{}
