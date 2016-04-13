// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_entry.hpp"
#include "dictionary.hpp"
#include "os_specific.hpp"
#include "string_ops.hpp"


// Private Member Functions 
void mousse::primitiveEntry::append(const UList<token>& varTokens)
{
  FOR_ALL(varTokens, i) {
    newElmt(tokenIndex()++) = varTokens[i];
  }
}


bool mousse::primitiveEntry::expandVariable
(
  const string& w,
  const dictionary& dict
)
{
  if (w.size() > 2 && w[0] == '$' && w[1] == token::BEGIN_BLOCK) {
    // Recursive substitution mode. Replace between {} with expansion.
    string s{w(2, w.size()-3)};
    // Substitute dictionary and environment variables. Do not allow
    // empty substitutions.
    stringOps::inplaceExpand(s, dict, true, false);
    string newW{w};
    newW.std::string::replace(1, newW.size()-1, s);
    return expandVariable(newW, dict);
  } else {
    string varName = w(1, w.size()-1);
    // lookup the variable name in the given dictionary....
    // Note: allow wildcards to match? For now disabled since following
    // would expand internalField to wildcard match and not expected
    // internalField:
    //      internalField XXX;
    //      boundaryField { ".*" {YYY;} movingWall {value $internalField;}
    const entry* ePtr = dict.lookupScopedEntryPtr(varName, true, false);
    // ...if defined append its tokens into this
    if (ePtr) {
      if (ePtr->isDict()) {
        append(ePtr->dict().tokens());
      } else {
        append(ePtr->stream());
      }
    } else {
      // not in the dictionary - try an environment variable
      string envStr = getEnv(varName);
      if (envStr.empty())
      {
        FATAL_IO_ERROR_IN
        (
          "primitiveEntry::expandVariable"
          "(const string&, const dictionary&",
          dict
        )
        << "Illegal dictionary entry or environment variable name "
        << varName << endl << "Valid dictionary entries are "
        << dict.toc() << exit(FatalIOError);
        return false;
      }
      append(tokenList(IStringStream('(' + envStr + ')')()));
    }
  }
  return true;
}


// Constructors 
mousse::primitiveEntry::primitiveEntry(const keyType& key, const ITstream& is)
:
  entry{key},
  ITstream{is}
{
  name() += '.' + keyword();
}


mousse::primitiveEntry::primitiveEntry(const keyType& key, const token& t)
:
  entry{key},
  ITstream{key, tokenList(1, t)}
{}


mousse::primitiveEntry::primitiveEntry
(
  const keyType& key,
  const UList<token>& tokens
)
:
  entry{key},
  ITstream{key, tokens}
{}


mousse::primitiveEntry::primitiveEntry
(
  const keyType& key,
  const Xfer<List<token> >& tokens
)
:
  entry{key},
  ITstream{key, tokens}
{}


// Member Functions 
mousse::label mousse::primitiveEntry::startLineNumber() const
{
  const tokenList& tokens = *this;
  if (tokens.empty()) {
    return -1;
  } else {
    return tokens.first().lineNumber();
  }
}


mousse::label mousse::primitiveEntry::endLineNumber() const
{
  const tokenList& tokens = *this;
  if (tokens.empty()) {
    return -1;
  } else {
    return tokens.last().lineNumber();
  }
}


mousse::ITstream& mousse::primitiveEntry::stream() const
{
  ITstream& is = const_cast<primitiveEntry&>(*this);
  is.rewind();
  return is;
}


const mousse::dictionary& mousse::primitiveEntry::dict() const
{
  FATAL_ERROR_IN("const dictionary& primitiveEntry::dict() const")
    << "Attempt to return primitive entry " << info()
    << " as a sub-dictionary"
    << abort(FatalError);
  return dictionary::null;
}


mousse::dictionary& mousse::primitiveEntry::dict()
{
  FATAL_ERROR_IN("const dictionary& primitiveEntry::dict()")
    << "Attempt to return primitive entry " << info()
    << " as a sub-dictionary"
    << abort(FatalError);
  return const_cast<dictionary&>(dictionary::null);
}
