// mousse: CFD toolbox
// Copyright (C) 2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "patch_dist_method.hpp"

// Static Data Members
namespace mousse
{
DEFINE_TYPE_NAME_AND_DEBUG(patchDistMethod, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(patchDistMethod, dictionary);
}

// Constructors
mousse::patchDistMethod::patchDistMethod
(
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
:
  mesh_(mesh),
  patchIDs_(patchIDs)
{}
// Selector
mousse::autoPtr<mousse::patchDistMethod> mousse::patchDistMethod::New
(
  const dictionary& dict,
  const fvMesh& mesh,
  const labelHashSet& patchIDs
)
{
  word patchDistMethodType(dict.lookup("method"));
  Info<< "Selecting patchDistMethod " << patchDistMethodType << endl;
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(patchDistMethodType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN("patchDistMethod::New")
      << "Unknown patchDistMethodType type "
      << patchDistMethodType << endl << endl
      << "Valid patchDistMethod types are : " << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << exit(FatalError);
  }
  return cstrIter()(dict, mesh, patchIDs);
}
// Destructor
mousse::patchDistMethod::~patchDistMethod()
{}
