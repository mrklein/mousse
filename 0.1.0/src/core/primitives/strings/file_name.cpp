// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project

#include "file_name.hpp"
#include "word_list.hpp"
#include "dynamic_list.hpp"
#include "debug.hpp"
#include "os_specific.hpp"
// Static Data Members
const char* const mousse::fileName::typeName = "fileName";
int mousse::fileName::debug(debug::debugSwitch(fileName::typeName, 0));
const mousse::fileName mousse::fileName::null;
// Constructors 
mousse::fileName::fileName(const wordList& lst)
{
  FOR_ALL(lst, elemI)
  {
    operator=((*this)/lst[elemI]);
  }
}
// Member Functions 
mousse::fileName::Type mousse::fileName::type() const
{
  return ::mousse::type(*this);
}
bool mousse::fileName::isAbsolute() const
{
  return !empty() && operator[](0) == '/';
}
mousse::fileName& mousse::fileName::toAbsolute()
{
  fileName& f = *this;
  if (!f.isAbsolute())
  {
    f = cwd()/f;
    f.clean();
  }
  return f;
}
//
// * remove repeated slashes
//       /abc////def        -->   /abc/def
//
// * remove '/./'
//       /abc/def/./ghi/.   -->   /abc/def/./ghi
//       abc/def/./         -->   abc/def
//
// * remove '/../'
//       /abc/def/../ghi/jkl/nmo/..   -->   /abc/ghi/jkl
//       abc/../def/ghi/../jkl        -->   abc/../def/jkl
//
// * remove trailing '/'
//
bool mousse::fileName::clean()
{
  // the top slash - we are never allowed to go above it
  string::size_type top = this->find('/');
  // no slashes - nothing to do
  if (top == string::npos)
  {
    return false;
  }
  // start with the '/' found:
  char prev = '/';
  string::size_type nChar  = top+1;
  string::size_type maxLen = this->size();
  for
  (
    string::size_type src = nChar;
    src < maxLen;
    /*nil*/
  )
  {
    char c = operator[](src++);
    if (prev == '/')
    {
      // repeated '/' - skip it
      if (c == '/')
      {
        continue;
      }
      // could be '/./' or '/../'
      if (c == '.')
      {
        // found trailing '/.' - skip it
        if (src >= maxLen)
        {
          continue;
        }
        // peek at the next character
        char c1 = operator[](src);
        // found '/./' - skip it
        if (c1 == '/')
        {
          src++;
          continue;
        }
        // it is '/..' or '/../'
        if (c1 == '.' && (src+1 >= maxLen || operator[](src+1) == '/'))
        {
          string::size_type parent;
          // backtrack to find the parent directory
          // minimum of 3 characters:  '/x/../'
          // strip it, provided it is above the top point
          if
          (
            nChar > 2
          && (parent = this->rfind('/', nChar-2)) != string::npos
          && parent >= top
          )
          {
            nChar = parent + 1;   // retain '/' from the parent
            src += 2;
            continue;
          }
          // bad resolution, eg 'abc/../../'
          // retain the sequence, but move the top to avoid it being
          // considered a valid parent later
          top = nChar + 2;
        }
      }
    }
    operator[](nChar++) = prev = c;
  }
  // remove trailing slash
  if (nChar > 1 && operator[](nChar-1) == '/')
  {
    nChar--;
  }
  this->resize(nChar);
  return (nChar != maxLen);
}
mousse::fileName mousse::fileName::clean() const
{
  fileName fName(*this);
  fName.clean();
  return fName;
}
//  Return file name (part beyond last /)
//
//  behaviour compared to /usr/bin/basename:
//    input           name()          basename
//    -----           ------          --------
//    "foo"           "foo"           "foo"
//    "/foo"          "foo"           "foo"
//    "foo/bar"       "bar"           "bar"
//    "/foo/bar"      "bar"           "bar"
//    "/foo/bar/"     ""              "bar"
//
mousse::word mousse::fileName::name() const
{
  size_type i = rfind('/');
  if (i == npos)
  {
    return *this;
  }
  else
  {
    return substr(i+1, npos);
  }
}
mousse::string mousse::fileName::caseName() const
{
  string cName = *this;
  const string caseStr(getEnv("MOUSSE_CASE"));
  const size_type i = find(caseStr);
  if (i == npos)
  {
    return cName;
  }
  else
  {
    return cName.replace(i, caseStr.size(), string("$MOUSSE_CASE"));
  }
}
mousse::word mousse::fileName::name(const bool noExt) const
{
  if (noExt)
  {
    size_type beg = rfind('/');
    if (beg == npos)
    {
      beg = 0;
    }
    else
    {
      ++beg;
    }
    size_type dot = rfind('.');
    if (dot != npos && dot <= beg)
    {
      dot = npos;
    }
    if (dot == npos)
    {
      return substr(beg, npos);
    }
    else
    {
      return substr(beg, dot - beg);
    }
  }
  else
  {
    return this->name();
  }
}
//  Return directory path name (part before last /)
//
//  behaviour compared to /usr/bin/dirname:
//    input           path()          dirname
//    -----           ------          -------
//    "foo"           "."             "."
//    "/foo"          "/"             "foo"
//    "foo/bar"       "foo"           "foo"
//    "/foo/bar"      "/foo"          "/foo"
//    "/foo/bar/"     "/foo/bar/"     "/foo"
//
mousse::fileName mousse::fileName::path() const
{
  size_type i = rfind('/');
  if (i == npos)
  {
    return ".";
  }
  else if (i)
  {
    return substr(0, i);
  }
  else
  {
    return "/";
  }
}
//  Return file name without extension (part before last .)
mousse::fileName mousse::fileName::lessExt() const
{
  size_type i = find_last_of("./");
  if (i == npos || i == 0 || operator[](i) == '/')
  {
    return *this;
  }
  else
  {
    return substr(0, i);
  }
}
//  Return file name extension (part after last .)
mousse::word mousse::fileName::ext() const
{
  size_type i = find_last_of("./");
  if (i == npos || i == 0 || operator[](i) == '/')
  {
    return word::null;
  }
  else
  {
    return substr(i+1, npos);
  }
}
// Return the components of the file name as a wordList
// note that concatenating the components will not necessarily retrieve
// the original input fileName
//
//  behaviour
//    input           components()
//    -----           ------
//    "foo"           1("foo")
//    "/foo"          1("foo")
//    "foo/bar"       2("foo", "bar")
//    "/foo/bar"      2("foo", "bar")
//    "/foo/bar/"     2("foo", "bar")
//
mousse::wordList mousse::fileName::components(const char delimiter) const
{
  DynamicList<word> wrdList(20);
  size_type beg=0, end=0;
  while ((end = find(delimiter, beg)) != npos)
  {
    // avoid empty element (caused by doubled slashes)
    if (beg < end)
    {
      wrdList.append(substr(beg, end-beg));
    }
    beg = end + 1;
  }
  // avoid empty trailing element
  if (beg < size())
  {
    wrdList.append(substr(beg, npos));
  }
  // transfer to wordList
  return wordList(wrdList.xfer());
}
// Return a component of the file name
mousse::word mousse::fileName::component
(
  const size_type cmpt,
  const char delimiter
) const
{
  return components(delimiter)[cmpt];
}
// Member Operators 
const mousse::fileName& mousse::fileName::operator=(const fileName& str)
{
  string::operator=(str);
  return *this;
}
const mousse::fileName& mousse::fileName::operator=(const word& str)
{
  string::operator=(str);
  return *this;
}
const mousse::fileName& mousse::fileName::operator=(const string& str)
{
  string::operator=(str);
  stripInvalid();
  return *this;
}
const mousse::fileName& mousse::fileName::operator=(const std::string& str)
{
  string::operator=(str);
  stripInvalid();
  return *this;
}
const mousse::fileName& mousse::fileName::operator=(const char* str)
{
  string::operator=(str);
  stripInvalid();
  return *this;
}
// Friend Operators 
mousse::fileName mousse::operator/(const string& a, const string& b)
{
  if (a.size())           // First string non-null
  {
    if (b.size())       // Second string non-null
    {
      return fileName(a + '/' + b);
    }
    else                // Second string null
    {
      return a;
    }
  }
  else                    // First string null
  {
    if (b.size())       // Second string non-null
    {
      return b;
    }
    else                // Second string null
    {
      return fileName();
    }
  }
}
