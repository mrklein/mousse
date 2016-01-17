// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "table.hpp"
// Constructors 
template<class Type>
mousse::Table<Type>::Table(const word& entryName, const dictionary& dict)
:
  TableBase<Type>(entryName, dict)
{
  Istream& is(dict.lookup(entryName));
  word entryType(is);
  token firstToken(is);
  is.putBack(firstToken);
  if (firstToken == token::BEGIN_SQR)
  {
    is >> this->dimensions_;
  }
  is  >> this->table_;
  TableBase<Type>::check();
}
template<class Type>
mousse::Table<Type>::Table(const Table<Type>& tbl)
:
  TableBase<Type>(tbl)
{}
// Destructor 
template<class Type>
mousse::Table<Type>::~Table()
{}
