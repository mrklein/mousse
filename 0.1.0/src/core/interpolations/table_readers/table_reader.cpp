// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "table_reader.hpp"

// Static Data Members
template<class Type>
mousse::autoPtr<mousse::tableReader<Type> > mousse::tableReader<Type>::New
(
  const dictionary& spec
)
{
  const word readerType = spec.lookupOrDefault<word>
  (
    "readerType",
    "openFoam"
  );
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_
      ->find(readerType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "tableReader::New(const dictionary&)"
    )
    << "Unknown reader type " << readerType
    << nl << nl
    << "Valid reader types : " << nl
    << dictionaryConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }

  return autoPtr<tableReader<Type>>{cstrIter()(spec)};
}


// Constructors 
template<class Type>
mousse::tableReader<Type>::tableReader(const dictionary&)
{}


// Destructor 
template<class Type>
mousse::tableReader<Type>::~tableReader()
{}


// Member Functions 
template<class Type>
void mousse::tableReader<Type>::write(Ostream& os) const
{
  if (this->type() != "openFoam")
  {
    os.writeKeyword("readerType")
      << this->type() << token::END_STATEMENT << nl;
  }
}
