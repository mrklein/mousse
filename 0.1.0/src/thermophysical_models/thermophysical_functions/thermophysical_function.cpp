// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "error.hpp"
#include "thermophysical_function.hpp"
#include "hash_table.hpp"


namespace mousse {

DEFINE_TYPE_NAME_AND_DEBUG(thermophysicalFunction, 0);
DEFINE_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, Istream);
DEFINE_RUN_TIME_SELECTION_TABLE(thermophysicalFunction, dictionary);

}


// Constructors 
mousse::autoPtr<mousse::thermophysicalFunction> mousse::thermophysicalFunction::New
(
  Istream& is
)
{
  if (debug) {
    Info << "thermophysicalFunction::New(Istream&) : "
      << "constructing thermophysicalFunction"
      << endl;
  }
  const word thermophysicalFunctionType{is};
  IstreamConstructorTable::iterator cstrIter =
    IstreamConstructorTablePtr_->find(thermophysicalFunctionType);
  if (cstrIter == IstreamConstructorTablePtr_->end()) {
    FATAL_ERROR_IN("thermophysicalFunction::New(Istream&)")
      << "Unknown thermophysicalFunction type "
      << thermophysicalFunctionType
      << nl << nl
      << "Valid thermophysicalFunction types are :" << endl
      << IstreamConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<thermophysicalFunction>{cstrIter()(is)};
}


mousse::autoPtr<mousse::thermophysicalFunction> mousse::thermophysicalFunction::New
(
  const dictionary& dict
)
{
  if (debug) {
    Info << "thermophysicalFunction::New(const dictionary&) : "
      << "constructing thermophysicalFunction"
      << endl;
  }
  const word thermophysicalFunctionType{dict.lookup("functionType")};
  dictionaryConstructorTable::iterator cstrIter =
    dictionaryConstructorTablePtr_->find(thermophysicalFunctionType);
  if (cstrIter == IstreamConstructorTablePtr_->end()) {
    FATAL_ERROR_IN("thermophysicalFunction::New(const dictionary&)")
      << "Unknown thermophysicalFunction type "
      << thermophysicalFunctionType
      << nl << nl
      << "Valid thermophysicalFunction types are :" << endl
      << dictionaryConstructorTablePtr_->sortedToc()
      << abort(FatalError);
  }
  return autoPtr<thermophysicalFunction>{cstrIter()(dict)};
}

