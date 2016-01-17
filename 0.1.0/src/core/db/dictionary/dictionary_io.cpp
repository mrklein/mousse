// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary.hpp"
#include "ifstream.hpp"
#include "input_mode_entry.hpp"
#include "reg_exp.hpp"

// Constructors 
mousse::dictionary::dictionary
(
  const fileName& name,
  const dictionary& parentDict,
  Istream& is
)
:
  dictionaryName{parentDict.name() + '.' + name},
  parent_{parentDict}
{
  read(is);
}


mousse::dictionary::dictionary(Istream& is)
:
  dictionaryName{is.name()},
  parent_{dictionary::null}
{
  // Reset input mode as this is a "top-level" dictionary
  functionEntries::inputModeEntry::clear();
  read(is);
}


mousse::dictionary::dictionary(Istream& is, const bool keepHeader)
:
  dictionaryName{is.name()},
  parent_{dictionary::null}
{
  // Reset input mode as this is a "top-level" dictionary
  functionEntries::inputModeEntry::clear();
  read(is, keepHeader);
}


// Selectors
mousse::autoPtr<mousse::dictionary> mousse::dictionary::New(Istream& is)
{
  return autoPtr<dictionary>{new dictionary{is}};
}


// Member Functions 
bool mousse::dictionary::read(Istream& is, const bool keepHeader)
{
  // Check for empty dictionary
  if (is.eof())
  {
    return true;
  }
  if (!is.good())
  {
    FATAL_IO_ERROR_IN("dictionary::read(Istream&, bool)", is)
      << "Istream not OK for reading dictionary "
      << exit(FatalIOError);
    return false;
  }
  token currToken(is);
  if (currToken != token::BEGIN_BLOCK)
  {
    is.putBack(currToken);
  }
  while (!is.eof() && entry::New(*this, is))
  {}
  // normally remove the FoamFile header entry if it exists
  if (!keepHeader)
  {
    remove("FoamFile");
  }
  if (is.bad())
  {
    Info<< "dictionary::read(Istream&, bool) : "
      << "Istream not OK after reading dictionary " << name()
      << endl;
    return false;
  }
  return true;
}
bool mousse::dictionary::read(Istream& is)
{
  return this->read(is, false);
}
bool mousse::dictionary::substituteKeyword(const word& keyword)
{
  word varName = keyword(1, keyword.size()-1);
  // lookup the variable name in the given dictionary
  const entry* ePtr = lookupEntryPtr(varName, true, true);
  // if defined insert its entries into this dictionary
  if (ePtr != NULL)
  {
    const dictionary& addDict = ePtr->dict();
    FOR_ALL_CONST_ITER(IDLList<entry>, addDict, iter)
    {
      add(iter());
    }
    return true;
  }
  return false;
}
// Istream Operator 
mousse::Istream& mousse::operator>>(Istream& is, dictionary& dict)
{
  // Reset input mode assuming this is a "top-level" dictionary
  functionEntries::inputModeEntry::clear();
  dict.clear();
  dict.name() = is.name();
  dict.read(is);
  return is;
}
// Ostream Operator 
void mousse::dictionary::write(Ostream& os, bool subDict) const
{
  if (subDict)
  {
    os << nl << indent << token::BEGIN_BLOCK << incrIndent << nl;
  }
  FOR_ALL_CONST_ITER(IDLList<entry>, *this, iter)
  {
    const entry& e = *iter;
    // Write entry
    os << e;
    // Add extra new line between entries for "top-level" dictionaries
    if (!subDict && parent() == dictionary::null && e != *last())
    {
      os << nl;
    }
    // Check stream before going to next entry.
    if (!os.good())
    {
      WARNING_IN("dictionary::write(Ostream&, bool subDict)")
        << "Can't write entry " << iter().keyword()
        << " for dictionary " << name()
        << endl;
    }
  }
  if (subDict)
  {
    os  << decrIndent << indent << token::END_BLOCK << endl;
  }
}
mousse::Ostream& mousse::operator<<(Ostream& os, const dictionary& dict)
{
  dict.write(os, true);
  return os;
}
