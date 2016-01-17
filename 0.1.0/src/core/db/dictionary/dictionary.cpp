// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "dictionary.hpp"
#include "primitive_entry.hpp"
#include "dictionary_entry.hpp"
#include "reg_exp.hpp"
#include "osha1stream.hpp"
#include "dynamic_list.hpp"
/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */
namespace mousse
{
  DEFINE_TYPE_NAME_AND_DEBUG(dictionary, 0);
  const dictionary dictionary::null;
  bool dictionary::writeOptionalEntries
  (
    debug::infoSwitch("writeOptionalEntries", 0)
  );
}
// Private Member Functions 
bool mousse::dictionary::findInPatterns
(
  const bool patternMatch,
  const word& Keyword,
  DLList<entry*>::const_iterator& wcLink,
  DLList<autoPtr<regExp> >::const_iterator& reLink
) const
{
  if (patternEntries_.size())
  {
    while (wcLink != patternEntries_.end())
    {
      if
      (
        patternMatch
       ? reLink()->match(Keyword)
       : wcLink()->keyword() == Keyword
      )
      {
        return true;
      }
      ++reLink;
      ++wcLink;
    }
  }
  return false;
}
bool mousse::dictionary::findInPatterns
(
  const bool patternMatch,
  const word& Keyword,
  DLList<entry*>::iterator& wcLink,
  DLList<autoPtr<regExp> >::iterator& reLink
)
{
  if (patternEntries_.size())
  {
    while (wcLink != patternEntries_.end())
    {
      if
      (
        patternMatch
       ? reLink()->match(Keyword)
       : wcLink()->keyword() == Keyword
      )
      {
        return true;
      }
      ++reLink;
      ++wcLink;
    }
  }
  return false;
}
// Constructors 
mousse::dictionary::dictionary()
:
  parent_(dictionary::null)
{}
mousse::dictionary::dictionary(const fileName& name)
:
  dictionaryName(name),
  parent_(dictionary::null)
{}
mousse::dictionary::dictionary
(
  const dictionary& parentDict,
  const dictionary& dict
)
:
  dictionaryName(dict.name()),
  IDLList<entry>(dict, *this),
  parent_(parentDict)
{
  FOR_ALL_ITER(IDLList<entry>, *this, iter)
  {
    hashedEntries_.insert(iter().keyword(), &iter());
    if (iter().keyword().isPattern())
    {
      patternEntries_.insert(&iter());
      patternRegexps_.insert
      (
        autoPtr<regExp>(new regExp(iter().keyword()))
      );
    }
  }
}
mousse::dictionary::dictionary
(
  const dictionary& dict
)
:
  dictionaryName(dict.name()),
  IDLList<entry>(dict, *this),
  parent_(dictionary::null)
{
  FOR_ALL_ITER(IDLList<entry>, *this, iter)
  {
    hashedEntries_.insert(iter().keyword(), &iter());
    if (iter().keyword().isPattern())
    {
      patternEntries_.insert(&iter());
      patternRegexps_.insert
      (
        autoPtr<regExp>(new regExp(iter().keyword()))
      );
    }
  }
}
mousse::dictionary::dictionary
(
  const dictionary* dictPtr
)
:
  parent_(dictionary::null)
{
  if (dictPtr)
  {
    operator=(*dictPtr);
  }
}
mousse::dictionary::dictionary
(
  const dictionary& parentDict,
  const Xfer<dictionary>& dict
)
:
  parent_(parentDict)
{
  transfer(dict());
  name() = parentDict.name() + '.' + name();
}
mousse::dictionary::dictionary
(
  const Xfer<dictionary>& dict
)
:
  parent_(dictionary::null)
{
  transfer(dict());
}
mousse::autoPtr<mousse::dictionary> mousse::dictionary::clone() const
{
  return autoPtr<dictionary>(new dictionary(*this));
}
// Destructor 
mousse::dictionary::~dictionary()
{
  // cerr<< "~dictionary() " << name() << " " << long(this) << std::endl;
}
// Member Functions 
const mousse::dictionary& mousse::dictionary::topDict() const
{
  const dictionary& p = parent();
  if (&p != this && !p.name().empty())
  {
    return p.topDict();
  }
  else
  {
    return p;
  }
}
mousse::label mousse::dictionary::startLineNumber() const
{
  if (size())
  {
    return first()->startLineNumber();
  }
  else
  {
    return -1;
  }
}
mousse::label mousse::dictionary::endLineNumber() const
{
  if (size())
  {
    return last()->endLineNumber();
  }
  else
  {
    return -1;
  }
}
mousse::SHA1Digest mousse::dictionary::digest() const
{
  OSHA1stream os;
  // process entries
  FOR_ALL_CONST_ITER(IDLList<entry>, *this, iter)
  {
    os << *iter;
  }
  return os.digest();
}
mousse::tokenList mousse::dictionary::tokens() const
{
  // linearise dictionary into a string
  OStringStream os;
  write(os, false);
  IStringStream is(os.str());
  // parse string as tokens
  DynamicList<token> tokens;
  token t;
  while (is.read(t))
  {
    tokens.append(t);
  }
  return tokenList(tokens.xfer());
}
bool mousse::dictionary::found
(
  const word& keyword,
  bool recursive,
  bool patternMatch
) const
{
  if (hashedEntries_.found(keyword))
  {
    return true;
  }
  else
  {
    if (patternMatch && patternEntries_.size())
    {
      DLList<entry*>::const_iterator wcLink =
        patternEntries_.begin();
      DLList<autoPtr<regExp> >::const_iterator reLink =
        patternRegexps_.begin();
      // Find in patterns using regular expressions only
      if (findInPatterns(patternMatch, keyword, wcLink, reLink))
      {
        return true;
      }
    }
    if (recursive && &parent_ != &dictionary::null)
    {
      return parent_.found(keyword, recursive, patternMatch);
    }
    else
    {
      return false;
    }
  }
}
const mousse::entry* mousse::dictionary::lookupEntryPtr
(
  const word& keyword,
  bool recursive,
  bool patternMatch
) const
{
  HashTable<entry*>::const_iterator iter = hashedEntries_.find(keyword);
  if (iter == hashedEntries_.end())
  {
    if (patternMatch && patternEntries_.size())
    {
      DLList<entry*>::const_iterator wcLink =
        patternEntries_.begin();
      DLList<autoPtr<regExp> >::const_iterator reLink =
        patternRegexps_.begin();
      // Find in patterns using regular expressions only
      if (findInPatterns(patternMatch, keyword, wcLink, reLink))
      {
        return wcLink();
      }
    }
    if (recursive && &parent_ != &dictionary::null)
    {
      return parent_.lookupEntryPtr(keyword, recursive, patternMatch);
    }
    else
    {
      return NULL;
    }
  }
  return iter();
}
mousse::entry* mousse::dictionary::lookupEntryPtr
(
  const word& keyword,
  bool recursive,
  bool patternMatch
)
{
  HashTable<entry*>::iterator iter = hashedEntries_.find(keyword);
  if (iter == hashedEntries_.end())
  {
    if (patternMatch && patternEntries_.size())
    {
      DLList<entry*>::iterator wcLink =
        patternEntries_.begin();
      DLList<autoPtr<regExp> >::iterator reLink =
        patternRegexps_.begin();
      // Find in patterns using regular expressions only
      if (findInPatterns(patternMatch, keyword, wcLink, reLink))
      {
        return wcLink();
      }
    }
    if (recursive && &parent_ != &dictionary::null)
    {
      return const_cast<dictionary&>(parent_).lookupEntryPtr
      (
        keyword,
        recursive,
        patternMatch
      );
    }
    else
    {
      return NULL;
    }
  }
  return iter();
}
const mousse::entry& mousse::dictionary::lookupEntry
(
  const word& keyword,
  bool recursive,
  bool patternMatch
) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, recursive, patternMatch);
  if (entryPtr == NULL)
  {
    FATAL_IO_ERROR_IN
    (
      "dictionary::lookupEntry(const word&, bool, bool) const",
      *this
    )   << "keyword " << keyword << " is undefined in dictionary "
      << name()
      << exit(FatalIOError);
  }
  return *entryPtr;
}
mousse::ITstream& mousse::dictionary::lookup
(
  const word& keyword,
  bool recursive,
  bool patternMatch
) const
{
  return lookupEntry(keyword, recursive, patternMatch).stream();
}
const mousse::entry* mousse::dictionary::lookupScopedEntryPtr
(
  const word& keyword,
  bool recursive,
  bool patternMatch
) const
{
  if (keyword[0] == ':')
  {
    // Go up to top level
    const dictionary* dictPtr = this;
    while (&dictPtr->parent_ != &dictionary::null)
    {
      dictPtr = &dictPtr->parent_;
    }
    // At top. Recurse to find entries
    return dictPtr->lookupScopedEntryPtr
    (
      keyword.substr(1, keyword.size()-1),
      false,
      patternMatch
    );
  }
  else
  {
    string::size_type dotPos = keyword.find('.');
    if (dotPos == string::npos)
    {
      // Non-scoped lookup
      return lookupEntryPtr(keyword, recursive, patternMatch);
    }
    else
    {
      if (dotPos == 0)
      {
        // Starting with a '.'. Go up for every 2nd '.' found
        const dictionary* dictPtr = this;
        string::size_type begVar = dotPos + 1;
        string::const_iterator iter = keyword.begin() + begVar;
        string::size_type endVar = begVar;
        while
        (
          iter != keyword.end()
        && *iter == '.'
        )
        {
          ++iter;
          ++endVar;
          // Go to parent
          if (&dictPtr->parent_ == &dictionary::null)
          {
            FATAL_IO_ERROR_IN
            (
              "dictionary::lookupScopedEntryPtr"
              "(const word&, bool, bool)",
              *this
            )   << "No parent of current dictionary"
              << " when searching for "
              << keyword.substr(begVar, keyword.size()-begVar)
              << exit(FatalIOError);
          }
          dictPtr = &dictPtr->parent_;
        }
        return dictPtr->lookupScopedEntryPtr
        (
          keyword.substr(endVar),
          false,
          patternMatch
        );
      }
      else
      {
        // Extract the first word
        word firstWord = keyword.substr(0, dotPos);
        const entry* entPtr = lookupScopedEntryPtr
        (
          firstWord,
          false,          //recursive
          patternMatch
        );
        if (!entPtr)
        {
          FATAL_IO_ERROR_IN
          (
            "dictionary::lookupScopedEntryPtr"
            "(const word&, bool, bool)",
            *this
          )   << "keyword " << firstWord
            << " is undefined in dictionary "
            << name() << endl
            << "Valid keywords are " << keys()
            << exit(FatalIOError);
        }
        if (entPtr->isDict())
        {
          return entPtr->dict().lookupScopedEntryPtr
          (
            keyword.substr(dotPos, keyword.size()-dotPos),
            false,
            patternMatch
          );
        }
        else
        {
          return NULL;
        }
      }
    }
  }
}
bool mousse::dictionary::substituteScopedKeyword(const word& keyword)
{
  word varName = keyword(1, keyword.size()-1);
  // lookup the variable name in the given dictionary
  const entry* ePtr = lookupScopedEntryPtr(varName, true, true);
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
bool mousse::dictionary::isDict(const word& keyword) const
{
  // Find non-recursive with patterns
  const entry* entryPtr = lookupEntryPtr(keyword, false, true);
  if (entryPtr)
  {
    return entryPtr->isDict();
  }
  else
  {
    return false;
  }
}
const mousse::dictionary* mousse::dictionary::subDictPtr(const word& keyword) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, false, true);
  if (entryPtr)
  {
    return &entryPtr->dict();
  }
  else
  {
    return NULL;
  }
}
const mousse::dictionary& mousse::dictionary::subDict(const word& keyword) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, false, true);
  if (entryPtr == NULL)
  {
    FATAL_IO_ERROR_IN
    (
      "dictionary::subDict(const word& keyword) const",
      *this
    )   << "keyword " << keyword << " is undefined in dictionary "
      << name()
      << exit(FatalIOError);
  }
  return entryPtr->dict();
}
mousse::dictionary& mousse::dictionary::subDict(const word& keyword)
{
  entry* entryPtr = lookupEntryPtr(keyword, false, true);
  if (entryPtr == NULL)
  {
    FATAL_IO_ERROR_IN
    (
      "dictionary::subDict(const word& keyword)",
      *this
    )   << "keyword " << keyword << " is undefined in dictionary "
      << name()
      << exit(FatalIOError);
  }
  return entryPtr->dict();
}
mousse::dictionary mousse::dictionary::subOrEmptyDict
(
  const word& keyword,
  const bool mustRead
) const
{
  const entry* entryPtr = lookupEntryPtr(keyword, false, true);
  if (entryPtr == NULL)
  {
    if (mustRead)
    {
      FATAL_IO_ERROR_IN
      (
        "dictionary::subOrEmptyDict(const word& keyword, const bool)",
        *this
      )   << "keyword " << keyword << " is undefined in dictionary "
        << name()
        << exit(FatalIOError);
      return entryPtr->dict();
    }
    else
    {
      return dictionary(*this, dictionary(name() + '.' + keyword));
    }
  }
  else
  {
    return entryPtr->dict();
  }
}
mousse::wordList mousse::dictionary::toc() const
{
  wordList keys(size());
  label nKeys = 0;
  FOR_ALL_CONST_ITER(IDLList<entry>, *this, iter)
  {
    keys[nKeys++] = iter().keyword();
  }
  return keys;
}
mousse::List<mousse::keyType> mousse::dictionary::keys(bool patterns) const
{
  List<keyType> keys(size());
  label nKeys = 0;
  FOR_ALL_CONST_ITER(IDLList<entry>, *this, iter)
  {
    if (iter().keyword().isPattern() ? patterns : !patterns)
    {
      keys[nKeys++] = iter().keyword();
    }
  }
  keys.setSize(nKeys);
  return keys;
}
bool mousse::dictionary::add(entry* entryPtr, bool mergeEntry)
{
  HashTable<entry*>::iterator iter = hashedEntries_.find
  (
    entryPtr->keyword()
  );
  if (mergeEntry && iter != hashedEntries_.end())
  {
    // merge dictionary with dictionary
    if (iter()->isDict() && entryPtr->isDict())
    {
      iter()->dict().merge(entryPtr->dict());
      delete entryPtr;
      return true;
    }
    else
    {
      // replace existing dictionary with entry or vice versa
      IDLList<entry>::replace(iter(), entryPtr);
      delete iter();
      hashedEntries_.erase(iter);
      if (hashedEntries_.insert(entryPtr->keyword(), entryPtr))
      {
        entryPtr->name() = name() + '.' + entryPtr->keyword();
        if (entryPtr->keyword().isPattern())
        {
          patternEntries_.insert(entryPtr);
          patternRegexps_.insert
          (
            autoPtr<regExp>(new regExp(entryPtr->keyword()))
          );
        }
        return true;
      }
      else
      {
        IO_WARNING_IN("dictionary::add(entry*, bool)", (*this))
          << "problem replacing entry "<< entryPtr->keyword()
          << " in dictionary " << name() << endl;
        IDLList<entry>::remove(entryPtr);
        delete entryPtr;
        return false;
      }
    }
  }
  if (hashedEntries_.insert(entryPtr->keyword(), entryPtr))
  {
    entryPtr->name() = name() + '.' + entryPtr->keyword();
    IDLList<entry>::append(entryPtr);
    if (entryPtr->keyword().isPattern())
    {
      patternEntries_.insert(entryPtr);
      patternRegexps_.insert
      (
        autoPtr<regExp>(new regExp(entryPtr->keyword()))
      );
    }
    return true;
  }
  else
  {
    IO_WARNING_IN("dictionary::add(entry*, bool)", (*this))
      << "attempt to add entry "<< entryPtr->keyword()
      << " which already exists in dictionary " << name()
      << endl;
    delete entryPtr;
    return false;
  }
}
void mousse::dictionary::add(const entry& e, bool mergeEntry)
{
  add(e.clone(*this).ptr(), mergeEntry);
}
void mousse::dictionary::add(const keyType& k, const word& w, bool overwrite)
{
  add(new primitiveEntry(k, token(w)), overwrite);
}
void mousse::dictionary::add
(
  const keyType& k,
  const mousse::string& s,
  bool overwrite
)
{
  add(new primitiveEntry(k, token(s)), overwrite);
}
void mousse::dictionary::add(const keyType& k, const label l, bool overwrite)
{
  add(new primitiveEntry(k, token(l)), overwrite);
}
void mousse::dictionary::add(const keyType& k, const scalar s, bool overwrite)
{
  add(new primitiveEntry(k, token(s)), overwrite);
}
void mousse::dictionary::add
(
  const keyType& k,
  const dictionary& d,
  bool mergeEntry
)
{
  add(new dictionaryEntry(k, *this, d), mergeEntry);
}
void mousse::dictionary::set(entry* entryPtr)
{
  entry* existingPtr = lookupEntryPtr(entryPtr->keyword(), false, true);
  // clear dictionary so merge acts like overwrite
  if (existingPtr && existingPtr->isDict())
  {
    existingPtr->dict().clear();
  }
  add(entryPtr, true);
}
void mousse::dictionary::set(const entry& e)
{
  set(e.clone(*this).ptr());
}
void mousse::dictionary::set(const keyType& k, const dictionary& d)
{
  set(new dictionaryEntry(k, *this, d));
}
bool mousse::dictionary::remove(const word& Keyword)
{
  HashTable<entry*>::iterator iter = hashedEntries_.find(Keyword);
  if (iter != hashedEntries_.end())
  {
    // Delete from patterns first
    DLList<entry*>::iterator wcLink =
      patternEntries_.begin();
    DLList<autoPtr<regExp> >::iterator reLink =
      patternRegexps_.begin();
    // Find in pattern using exact match only
    if (findInPatterns(false, Keyword, wcLink, reLink))
    {
      patternEntries_.remove(wcLink);
      patternRegexps_.remove(reLink);
    }
    IDLList<entry>::remove(iter());
    delete iter();
    hashedEntries_.erase(iter);
    return true;
  }
  else
  {
    return false;
  }
}
bool mousse::dictionary::changeKeyword
(
  const keyType& oldKeyword,
  const keyType& newKeyword,
  bool forceOverwrite
)
{
  // no change
  if (oldKeyword == newKeyword)
  {
    return false;
  }
  HashTable<entry*>::iterator iter = hashedEntries_.find(oldKeyword);
  // oldKeyword not found - do nothing
  if (iter == hashedEntries_.end())
  {
    return false;
  }
  if (iter()->keyword().isPattern())
  {
    FATAL_IO_ERROR_IN
    (
      "dictionary::changeKeyword(const word&, const word&, bool)",
      *this
    )   << "Old keyword "<< oldKeyword
      << " is a pattern."
      << "Pattern replacement not yet implemented."
      << exit(FatalIOError);
  }
  HashTable<entry*>::iterator iter2 = hashedEntries_.find(newKeyword);
  // newKeyword already exists
  if (iter2 != hashedEntries_.end())
  {
    if (forceOverwrite)
    {
      if (iter2()->keyword().isPattern())
      {
        // Delete from patterns first
        DLList<entry*>::iterator wcLink =
          patternEntries_.begin();
        DLList<autoPtr<regExp> >::iterator reLink =
          patternRegexps_.begin();
        // Find in patterns using exact match only
        if (findInPatterns(false, iter2()->keyword(), wcLink, reLink))
        {
          patternEntries_.remove(wcLink);
          patternRegexps_.remove(reLink);
        }
      }
      IDLList<entry>::replace(iter2(), iter());
      delete iter2();
      hashedEntries_.erase(iter2);
    }
    else
    {
      IO_WARNING_IN
      (
        "dictionary::changeKeyword(const word&, const word&, bool)",
        *this
      )   << "cannot rename keyword "<< oldKeyword
        << " to existing keyword " << newKeyword
        << " in dictionary " << name() << endl;
      return false;
    }
  }
  // change name and HashTable, but leave DL-List untouched
  iter()->keyword() = newKeyword;
  iter()->name() = name() + '.' + newKeyword;
  hashedEntries_.erase(oldKeyword);
  hashedEntries_.insert(newKeyword, iter());
  if (newKeyword.isPattern())
  {
    patternEntries_.insert(iter());
    patternRegexps_.insert
    (
      autoPtr<regExp>(new regExp(newKeyword))
    );
  }
  return true;
}
bool mousse::dictionary::merge(const dictionary& dict)
{
  // Check for assignment to self
  if (this == &dict)
  {
    FATAL_IO_ERROR_IN("dictionary::merge(const dictionary&)", *this)
      << "attempted merge to self for dictionary " << name()
      << abort(FatalIOError);
  }
  bool changed = false;
  FOR_ALL_CONST_ITER(IDLList<entry>, dict, iter)
  {
    HashTable<entry*>::iterator fnd = hashedEntries_.find(iter().keyword());
    if (fnd != hashedEntries_.end())
    {
      // Recursively merge sub-dictionaries
      // TODO: merge without copying
      if (fnd()->isDict() && iter().isDict())
      {
        if (fnd()->dict().merge(iter().dict()))
        {
          changed = true;
        }
      }
      else
      {
        add(iter().clone(*this).ptr(), true);
        changed = true;
      }
    }
    else
    {
      // not found - just add
      add(iter().clone(*this).ptr());
      changed = true;
    }
  }
  return changed;
}
void mousse::dictionary::clear()
{
  IDLList<entry>::clear();
  hashedEntries_.clear();
  patternEntries_.clear();
  patternRegexps_.clear();
}
void mousse::dictionary::transfer(dictionary& dict)
{
  // changing parents probably doesn't make much sense,
  // but what about the names?
  name() = dict.name();
  IDLList<entry>::transfer(dict);
  hashedEntries_.transfer(dict.hashedEntries_);
  patternEntries_.transfer(dict.patternEntries_);
  patternRegexps_.transfer(dict.patternRegexps_);
}
mousse::Xfer<mousse::dictionary> mousse::dictionary::xfer()
{
  return xferMove(*this);
}
// Member Operators 
mousse::ITstream& mousse::dictionary::operator[](const word& keyword) const
{
  return lookup(keyword);
}
void mousse::dictionary::operator=(const dictionary& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_IO_ERROR_IN("dictionary::operator=(const dictionary&)", *this)
      << "attempted assignment to self for dictionary " << name()
      << abort(FatalIOError);
  }
  name() = rhs.name();
  clear();
  // Create clones of the entries in the given dictionary
  // resetting the parentDict to this dictionary
  FOR_ALL_CONST_ITER(IDLList<entry>, rhs, iter)
  {
    add(iter().clone(*this).ptr());
  }
}
void mousse::dictionary::operator+=(const dictionary& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_IO_ERROR_IN("dictionary::operator+=(const dictionary&)", *this)
      << "attempted addition assignment to self for dictionary " << name()
      << abort(FatalIOError);
  }
  FOR_ALL_CONST_ITER(IDLList<entry>, rhs, iter)
  {
    add(iter().clone(*this).ptr());
  }
}
void mousse::dictionary::operator|=(const dictionary& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_IO_ERROR_IN("dictionary::operator|=(const dictionary&)", *this)
      << "attempted assignment to self for dictionary " << name()
      << abort(FatalIOError);
  }
  FOR_ALL_CONST_ITER(IDLList<entry>, rhs, iter)
  {
    if (!found(iter().keyword()))
    {
      add(iter().clone(*this).ptr());
    }
  }
}
void mousse::dictionary::operator<<=(const dictionary& rhs)
{
  // Check for assignment to self
  if (this == &rhs)
  {
    FATAL_IO_ERROR_IN("dictionary::operator<<=(const dictionary&)", *this)
      << "attempted assignment to self for dictionary " << name()
      << abort(FatalIOError);
  }
  FOR_ALL_CONST_ITER(IDLList<entry>, rhs, iter)
  {
    set(iter().clone(*this).ptr());
  }
}
/* * * * * * * * * * * * * * * * Global operators  * * * * * * * * * * * * * */
mousse::dictionary mousse::operator+
(
  const dictionary& dict1,
  const dictionary& dict2
)
{
  dictionary sum(dict1);
  sum += dict2;
  return sum;
}
mousse::dictionary mousse::operator|
(
  const dictionary& dict1,
  const dictionary& dict2
)
{
  dictionary sum(dict1);
  sum |= dict2;
  return sum;
}
