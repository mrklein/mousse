// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"
#include "time.hpp"

// Constructor
template<class Type>
mousse::DataEntry<Type>::DataEntry(const word& entryName)
:
  refCount{},
  name_{entryName}
{}


template<class Type>
mousse::DataEntry<Type>::DataEntry(const DataEntry<Type>& de)
:
  refCount{},
  name_{de.name_}
{}


// Destructor 
template<class Type>
mousse::DataEntry<Type>::~DataEntry()
{}


// Member Functions 
template<class Type>
const mousse::word& mousse::DataEntry<Type>::name() const
{
  return name_;
}


template<class Type>
void mousse::DataEntry<Type>::convertTimeBase(const Time&)
{
  // do nothing
}


template<class Type>
Type mousse::DataEntry<Type>::value(const scalar) const
{
  NOT_IMPLEMENTED("Type mousse::DataEntry<Type>::value(const scalar) const");
  return pTraits<Type>::zero;
}


template<class Type>
Type mousse::DataEntry<Type>::integrate(const scalar, const scalar) const
{
  NOT_IMPLEMENTED
  (
    "Type mousse::DataEntry<Type>::integrate"
    "("
      "const scalar, "
      "const scalar"
    ") const"
  );
  return pTraits<Type>::zero;
}


template<class Type>
mousse::tmp<mousse::Field<Type>> mousse::DataEntry<Type>::value
(
  const scalarField& x
) const
{
  tmp<Field<Type>> tfld{new Field<Type>{x.size()}};
  Field<Type>& fld = tfld();
  FOR_ALL(x, i)
  {
    fld[i] = this->value(x[i]);
  }
  return tfld;
}


template<class Type>
mousse::tmp<mousse::Field<Type>> mousse::DataEntry<Type>::integrate
(
  const scalarField& x1,
  const scalarField& x2
) const
{
  tmp<Field<Type>> tfld(new Field<Type>{x1.size()});
  Field<Type>& fld = tfld();
  FOR_ALL(x1, i)
  {
    fld[i] = this->integrate(x1[i], x2[i]);
  }
  return tfld;
}


template<class Type>
mousse::dimensioned<Type> mousse::DataEntry<Type>::dimValue(const scalar) const
{
  NOT_IMPLEMENTED
  (
    "dimensioned<Type> mousse::DataEntry<dimensioned<Type>>::dimValue"
    "(const scalar) const"
  );
  return dimensioned<Type>{"zero", dimless, pTraits<Type>::zero};
}


template<class Type>
mousse::dimensioned<Type> mousse::DataEntry<Type>::dimIntegrate
(
  const scalar,
  const scalar
) const
{
  NOT_IMPLEMENTED
  (
    "dimensioned<Type> mousse::DataEntry<Type>::dimIntegrate"
    "("
      "const scalar, "
      "const scalar"
    ") const"
  );
  return dimensioned<Type>{"zero", dimless, pTraits<Type>::zero};
}


template<class Type>
mousse::tmp<mousse::Field<mousse::dimensioned<Type>>>
mousse::DataEntry<Type>::dimValue
(
  const scalarField& x
) const
{
  tmp<Field<dimensioned<Type>>> tfld
  {
    new Field<dimensioned<Type>>
    {
      x.size(),
      dimensioned<Type>{"zero", dimless, pTraits<Type>::zero}
    }
  };
  Field<dimensioned<Type>>& fld = tfld();
  FOR_ALL(x, i)
  {
    fld[i] = this->dimValue(x[i]);
  }
  return tfld;
}


template<class Type>
mousse::tmp<mousse::Field<mousse::dimensioned<Type>>>
mousse::DataEntry<Type>::dimIntegrate
(
  const scalarField& x1,
  const scalarField& x2
) const
{
  tmp<Field<dimensioned<Type>>> tfld
  {
    new Field<dimensioned<Type>>{x1.size()}
  };
  Field<dimensioned<Type>>& fld = tfld();
  FOR_ALL(x1, i)
  {
    fld[i] = this->dimIntegrate(x1[i], x2[i]);
  }
  return tfld;
}
//  IOStream operators
#include "data_entry_io.cpp"
