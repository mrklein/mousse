// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "string.hpp"
#include "string_ops.hpp"
/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */
const char* const mousse::string::typeName = "string";
int mousse::string::debug(mousse::debug::debugSwitch(string::typeName, 0));
const mousse::string mousse::string::null;
// Member Functions 
// Count and return the number of a given character in the string
mousse::string::size_type mousse::string::count(const char c) const
{
  size_type cCount = 0;
  for (const_iterator iter = begin(); iter != end(); ++iter)
  {
    if (*iter == c)
    {
      ++cCount;
    }
  }
  return cCount;
}
// Replace first occurence of sub-string oldStr with newStr
mousse::string& mousse::string::replace
(
  const string& oldStr,
  const string& newStr,
  size_type start
)
{
  size_type newStart = start;
  if ((newStart = find(oldStr, newStart)) != npos)
  {
    std::string::replace(newStart, oldStr.size(), newStr);
  }
  return *this;
}
// Replace all occurences of sub-string oldStr with newStr
mousse::string& mousse::string::replaceAll
(
  const string& oldStr,
  const string& newStr,
  size_type start
)
{
  if (oldStr.size())
  {
    size_type newStart = start;
    while ((newStart = find(oldStr, newStart)) != npos)
    {
      std::string::replace(newStart, oldStr.size(), newStr);
      newStart += newStr.size();
    }
  }
  return *this;
}
mousse::string& mousse::string::expand(const bool allowEmpty)
{
  stringOps::inplaceExpand(*this, allowEmpty);
  return *this;
}
// Remove repeated characters returning true if string changed
bool mousse::string::removeRepeated(const char character)
{
  bool changed = false;
  if (character && find(character) != npos)
  {
    string::size_type nChar=0;
    iterator iter2 = begin();
    char prev = 0;
    for
    (
      string::const_iterator iter1 = iter2;
      iter1 != end();
      iter1++
    )
    {
      char c = *iter1;
      if (prev == c && c == character)
      {
        changed = true;
      }
      else
      {
        *iter2 = prev = c;
        ++iter2;
        ++nChar;
      }
    }
    resize(nChar);
  }
  return changed;
}
// Return string with repeated characters removed
mousse::string mousse::string::removeRepeated(const char character) const
{
  string str(*this);
  str.removeRepeated(character);
  return str;
}
// Remove trailing character returning true if string changed
bool mousse::string::removeTrailing(const char character)
{
  bool changed = false;
  string::size_type nChar = size();
  if (character && nChar > 1 && operator[](nChar-1) == character)
  {
    resize(nChar-1);
    changed = true;
  }
  return changed;
}
// Return string with trailing character removed
mousse::string mousse::string::removeTrailing(const char character) const
{
  string str(*this);
  str.removeTrailing(character);
  return str;
}
