// mousse: CFD toolbox
// Copyright (C) 2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "demand_driven_entry.hpp"
template<class Type>
inline void mousse::demandDrivenEntry<Type>::initialise() const
{
  if (!stored_)
  {
    dict_.lookup(keyword_) >> value_;
    stored_ = true;
  }
}
template<class Type>
inline const Type& mousse::demandDrivenEntry<Type>::value() const
{
  initialise();
  return value_;
}
template<class Type>
inline void mousse::demandDrivenEntry<Type>::setValue(const Type& value)
{
//    dict_.set<Type>(keyword_, value);
  value_ = value;
  stored_ = true;
}
template<class Type>
inline void mousse::demandDrivenEntry<Type>::reset()
{
  stored_ = false;
}
