// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "les_filter.hpp"
// Static Data Members
namespace mousse
{
  defineTypeNameAndDebug(LESfilter, 0);
  defineRunTimeSelectionTable(LESfilter, dictionary);
}
// Selectors
mousse::autoPtr<mousse::LESfilter> mousse::LESfilter::New
(
  const fvMesh& mesh,
  const dictionary& dict,
  const word& filterDictName
)
{
  const word filterType(dict.lookup(filterDictName));
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(filterType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FatalErrorIn
    (
      "LESfilter::New(const fvMesh&, const dictionary&)"
    )   << "Unknown LESfilter type "
      << filterType << nl << nl
      << "Valid LESfilter types are :" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return autoPtr<LESfilter>(cstrIter()(mesh, dict));
}
