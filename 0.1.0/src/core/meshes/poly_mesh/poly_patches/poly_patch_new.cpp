// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "poly_patch.hpp"
#include "dictionary.hpp"

// Member Functions 
mousse::autoPtr<mousse::polyPatch> mousse::polyPatch::New
(
  const word& patchType,
  const word& name,
  const label size,
  const label start,
  const label index,
  const polyBoundaryMesh& bm
)
{
  if (debug)
  {
    Info<< "polyPatch::New(const word&, const word&, const label, "
       "const label, const label, const polyBoundaryMesh&) : "
       "constructing polyPatch"
      << endl;
  }
  wordConstructorTable::iterator cstrIter =
    wordConstructorTablePtr_->find(patchType);
  if (cstrIter == wordConstructorTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "polyPatch::New(const word&, const word&, const label, "
      "const label, const label, const polyBoundaryMesh&) "
    )
    << "Unknown polyPatch type "
    << patchType << " for patch " << name << nl << nl
    << "Valid polyPatch types are :" << endl
    << wordConstructorTablePtr_->sortedToc()
    << exit(FatalError);
  }
  return autoPtr<polyPatch>
  {
    cstrIter()
    (
      name,
      size,
      start,
      index,
      bm,
      patchType
    )
  };
}


mousse::autoPtr<mousse::polyPatch> mousse::polyPatch::New
(
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm
)
{
  if (debug)
  {
    Info<< "polyPatch::New(const word&, const dictionary&, const label, "
       "const polyBoundaryMesh&) : constructing polyPatch"
      << endl;
  }
  word patchType(dict.lookup("type"));
  dict.readIfPresent("geometricType", patchType);
  return polyPatch::New(patchType, name, dict, index, bm);
}


mousse::autoPtr<mousse::polyPatch> mousse::polyPatch::New
(
  const word& patchType,
  const word& name,
  const dictionary& dict,
  const label index,
  const polyBoundaryMesh& bm
)
{
  if (debug)
  {
    Info<< "polyPatch::New(const word&, const word&, const dictionary&, "
       "const label, const polyBoundaryMesh&) : constructing polyPatch"
      << endl;
  }
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(patchType);
  if (cstrIter == dictionaryConstructorTablePtr_->end())
  {
    if (!disallowGenericPolyPatch)
    {
      cstrIter = dictionaryConstructorTablePtr_->find("genericPatch");
    }
    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
      FATAL_IO_ERROR_IN
      (
        "polyPatch::New(const word&, const dictionary&, "
        "const label, const polyBoundaryMesh&)",
        dict
      )   << "Unknown polyPatch type "
        << patchType << " for patch " << name << nl << nl
        << "Valid polyPatch types are :" << endl
        << dictionaryConstructorTablePtr_->sortedToc()
        << exit(FatalIOError);
    }
  }
  return autoPtr<polyPatch>{cstrIter()(name, dict, index, bm, patchType)};
}
