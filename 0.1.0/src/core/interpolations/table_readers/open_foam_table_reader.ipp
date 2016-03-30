// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "open_foam_table_reader.hpp"
#include "ifstream.hpp"


// Constructors 
template<class Type>
mousse::openFoamTableReader<Type>::openFoamTableReader(const dictionary& dict)
:
  tableReader<Type>{dict}
{}


// Destructor 
template<class Type>
mousse::openFoamTableReader<Type>::~openFoamTableReader()
{}


// Member Functions 
template<class Type>
void mousse::openFoamTableReader<Type>::operator()
(
  const fileName& fName,
  List<Tuple2<scalar, Type> >& data
)
{
  // Read data from file
  IFstream(fName)() >> data;
}


template<class Type>
void mousse::openFoamTableReader<Type>::operator()
(
  const fileName& fName,
  List<Tuple2<scalar, List<Tuple2<scalar, Type> > > >& data
)
{
  // Read data from file
  IFstream(fName)() >> data;
}

