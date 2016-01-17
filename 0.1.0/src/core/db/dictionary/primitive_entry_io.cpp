// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "primitive_entry.hpp"
#include "function_entry.hpp"
#include "dictionary.hpp"
// Private Member Functions 
void mousse::primitiveEntry::append
(
  const token& currToken,
  const dictionary& dict,
  Istream& is
)
{
  if (currToken.isWord())
  {
    const word& w = currToken.wordToken();
    if
    (
      disableFunctionEntries
    || w.size() == 1
    || (
        !(w[0] == '$' && expandVariable(w, dict))
      && !(w[0] == '#' && expandFunction(w, dict, is))
      )
    )
    {
      newElmt(tokenIndex()++) = currToken;
    }
  }
  else if (currToken.isVariable())
  {
    const string& w = currToken.stringToken();
    if
    (
      disableFunctionEntries
    || w.size() <= 3
    || !(
        w[0] == '$'
      && w[1] == token::BEGIN_BLOCK
      && expandVariable(w, dict)
      )
    )
    {
      newElmt(tokenIndex()++) = currToken;
    }
  }
  else
  {
    newElmt(tokenIndex()++) = currToken;
  }
}
bool mousse::primitiveEntry::expandFunction
(
  const word& keyword,
  const dictionary& parentDict,
  Istream& is
)
{
  word functionName = keyword(1, keyword.size()-1);
  return functionEntry::execute(functionName, parentDict, *this, is);
}
bool mousse::primitiveEntry::read(const dictionary& dict, Istream& is)
{
  is.fatalCheck
  (
    "primitiveEntry::readData(const dictionary&, Istream&)"
  );
  label blockCount = 0;
  token currToken;
  if
  (
    !is.read(currToken).bad()
  && currToken.good()
  && currToken != token::END_STATEMENT
  )
  {
    append(currToken, dict, is);
    if
    (
      currToken == token::BEGIN_BLOCK
    || currToken == token::BEGIN_LIST
    )
    {
      blockCount++;
    }
    while
    (
      !is.read(currToken).bad()
    && currToken.good()
    && !(currToken == token::END_STATEMENT && blockCount == 0)
    )
    {
      if
      (
        currToken == token::BEGIN_BLOCK
      || currToken == token::BEGIN_LIST
      )
      {
        blockCount++;
      }
      else if
      (
        currToken == token::END_BLOCK
      || currToken == token::END_LIST
      )
      {
        blockCount--;
      }
      append(currToken, dict, is);
    }
  }
  is.fatalCheck
  (
    "primitiveEntry::readData(const dictionary&, Istream&)"
  );
  if (currToken.good())
  {
    return true;
  }
  else
  {
    return false;
  }
}
void mousse::primitiveEntry::readEntry(const dictionary& dict, Istream& is)
{
  label keywordLineNumber = is.lineNumber();
  tokenIndex() = 0;
  if (read(dict, is))
  {
    setSize(tokenIndex());
    tokenIndex() = 0;
  }
  else
  {
    std::ostringstream os;
    os<< "ill defined primitiveEntry starting at keyword '"
      << keyword() << '\''
      << " on line " << keywordLineNumber
      << " and ending at line " << is.lineNumber();
    SAFE_FATAL_IO_ERROR_IN
    (
      "primitiveEntry::readEntry(const dictionary&, Istream&)",
      is,
      os.str()
    );
  }
}
// Constructors 
mousse::primitiveEntry::primitiveEntry
(
  const keyType& key,
  const dictionary& dict,
  Istream& is
)
:
  entry(key),
  ITstream
  (
    is.name() + '.' + key,
    tokenList(10),
    is.format(),
    is.version()
  )
{
  readEntry(dict, is);
}
mousse::primitiveEntry::primitiveEntry(const keyType& key, Istream& is)
:
  entry(key),
  ITstream
  (
    is.name() + '.' + key,
    tokenList(10),
    is.format(),
    is.version()
  )
{
  readEntry(dictionary::null, is);
}
// Member Functions 
void mousse::primitiveEntry::write(Ostream& os, const bool contentsOnly) const
{
  if (!contentsOnly)
  {
    os.writeKeyword(keyword());
  }
  for (label i=0; i<size(); ++i)
  {
    const token& t = operator[](i);
    if (t.type() == token::VERBATIMSTRING)
    {
      // Bypass token output operator to avoid losing verbatimness.
      // Handle in Ostreams themselves
      os.write(t);
    }
    else
    {
      os  << t;
    }
    if (i < size()-1)
    {
      os  << token::SPACE;
    }
  }
  if (!contentsOnly)
  {
    os  << token::END_STATEMENT << endl;
  }
}
void mousse::primitiveEntry::write(Ostream& os) const
{
  this->write(os, false);
}
// Ostream operator 
template<>
mousse::Ostream& mousse::operator<<
(
  Ostream& os,
  const InfoProxy<primitiveEntry>& ip
)
{
  const primitiveEntry& e = ip.t_;
  e.print(os);
  const label nPrintTokens = 10;
  os  << "    primitiveEntry '" << e.keyword() << "' comprises ";
  for (label i=0; i<min(e.size(), nPrintTokens); i++)
  {
    os  << nl << "        " << e[i].info();
  }
  if (e.size() > nPrintTokens)
  {
    os  << " ...";
  }
  os  << endl;
  return os;
}
