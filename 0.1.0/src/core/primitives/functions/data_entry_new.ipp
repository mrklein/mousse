// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "data_entry.hpp"


template<class Type>
mousse::autoPtr<mousse::DataEntry<Type>> mousse::DataEntry<Type>::New
(
  const word& entryName,
  const dictionary& dict
)
{
  Istream& is = dict.lookup(entryName, false);
  token firstToken{is};
  word DataEntryType;
  if (firstToken.isWord())
  {
    // Dimensioned type default compatibility
    if (firstToken.wordToken() == entryName)
    {
      DataEntryType = "CompatibilityConstant";
    }
    else
    {
      DataEntryType = firstToken.wordToken();
    }
  }
  else
  {
    // DataEntryType = CompatibilityConstant<Type>::typeName;
    DataEntryType = "CompatibilityConstant";
  }
  typename dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(DataEntryType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("DataEntry<Type>::New(const word&, const dictionary&)")
      << "Unknown DataEntry type "
      << DataEntryType << " for DataEntry "
      << entryName << nl << nl
      << "Valid DataEntry types are:" << nl
      << dictionaryConstructorTablePtr_->sortedToc() << nl
      << exit(FatalError);
  }
  return autoPtr<DataEntry<Type>>{cstrIter()(entryName, dict)};
}
