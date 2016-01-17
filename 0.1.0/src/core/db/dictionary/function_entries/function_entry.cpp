// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "function_entry.hpp"
#include "list.hpp"
namespace mousse
{
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE
  (
    functionEntry,
    execute,
    dictionaryIstream
  );
  DEFINE_MEMBER_FUNCTION_SELECTION_TABLE
  (
    functionEntry,
    execute,
    primitiveEntryIstream
  );
}
// Member Function Selectors
bool mousse::functionEntry::execute
(
  const word& functionName,
  dictionary& parentDict,
  Istream& is
)
{
  is.fatalCheck
  (
    "functionEntry::execute"
    "(const word& functionName, dictionary& parentDict, Istream&)"
  );
  if (!executedictionaryIstreamMemberFunctionTablePtr_)
  {
    cerr<< "functionEntry::execute"
      << "(const word&, dictionary&, Istream&)"
      << " not yet initialized, function = "
      << functionName.c_str() << std::endl;
    // Return true to keep reading
    return true;
  }
  executedictionaryIstreamMemberFunctionTable::iterator mfIter =
    executedictionaryIstreamMemberFunctionTablePtr_->find(functionName);
  if (mfIter == executedictionaryIstreamMemberFunctionTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "functionEntry::execute"
      "(const word& functionName, dictionary& parentDict, Istream&)"
    )   << "Unknown functionEntry '" << functionName
      << "' in " << is.name() << " near line " << is.lineNumber()
      << nl << nl
      << "Valid functionEntries are :" << endl
      << executedictionaryIstreamMemberFunctionTablePtr_->toc()
      << exit(FatalError);
  }
  return mfIter()(parentDict, is);
}
bool mousse::functionEntry::execute
(
  const word& functionName,
  const dictionary& parentDict,
  primitiveEntry& entry,
  Istream& is
)
{
  is.fatalCheck
  (
    "functionEntry::execute"
    "(const word&, const dictionary&, primitiveEntry&, Istream&)"
  );
  if (!executeprimitiveEntryIstreamMemberFunctionTablePtr_)
  {
    cerr<< "functionEntry::execute"
      << "(const word&, const dictionary&, primitiveEntry&, Istream&)"
      << " not yet initialized, function = "
      << functionName.c_str() << std::endl;
    // return true to keep reading anyhow
    return true;
  }
  executeprimitiveEntryIstreamMemberFunctionTable::iterator mfIter =
    executeprimitiveEntryIstreamMemberFunctionTablePtr_->find(functionName);
  if (mfIter == executeprimitiveEntryIstreamMemberFunctionTablePtr_->end())
  {
    FATAL_ERROR_IN
    (
      "functionEntry::execute"
      "(const word&, const dictionary&, primitiveEntry&, Istream&)"
    )   << "Unknown functionEntry '" << functionName
      << "' in " << is.name() << " near line " << is.lineNumber()
      << nl << nl
      << "Valid functionEntries are :" << endl
      << executeprimitiveEntryIstreamMemberFunctionTablePtr_->toc()
      << exit(FatalError);
  }
  return mfIter()(parentDict, entry, is);
}
