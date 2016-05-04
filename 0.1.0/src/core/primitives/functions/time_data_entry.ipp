// mousse: CFD toolbox
// Copyright (C) 2012-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "time_data_entry.hpp"


// Constructor
template<class Type>
mousse::TimeDataEntry<Type>::TimeDataEntry
(
  const Time& t,
  const word& name,
  const dictionary& dict
)
:
  time_{t},
  name_{name},
  entry_{DataEntry<Type>::New(name, dict)}
{
  entry_->convertTimeBase(t);
}


template<class Type>
mousse::TimeDataEntry<Type>::TimeDataEntry(const Time& t, const word& name)
:
  time_{t},
  name_{name},
  entry_{nullptr}
{}


template<class Type>
mousse::TimeDataEntry<Type>::TimeDataEntry
(
  const TimeDataEntry<Type>& tde
)
:
  time_{tde.time_},
  name_{tde.name_},
  entry_{}
{
  if (tde.entry_.valid())
  {
    entry_.reset(tde.entry_->clone().ptr());
  }
}


// Destructor 
template<class Type>
mousse::TimeDataEntry<Type>::~TimeDataEntry()
{}


// Member Functions 
template<class Type>
void mousse::TimeDataEntry<Type>::reset(const dictionary& dict)
{
  entry_.reset
  (
    DataEntry<Type>::New
    (
      name_,
      dict
    ).ptr()
  );
  entry_->convertTimeBase(time_);
}


template<class Type>
const mousse::word& mousse::TimeDataEntry<Type>::name() const
{
  return entry_->name();
}


template<class Type>
Type mousse::TimeDataEntry<Type>::value(const scalar x) const
{
  return entry_->value(x);
}


template<class Type>
Type mousse::TimeDataEntry<Type>::integrate
(
  const scalar x1,
  const scalar x2
) const
{
  return entry_->integrate(x1, x2);
}


// IOstream Operators 
template<class Type>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const TimeDataEntry<Type>& de
)
{
  return de.entry_->operator<<(os, de);
}


template<class Type>
void mousse::TimeDataEntry<Type>::writeData(Ostream& os) const
{
  entry_->writeData(os);
}

