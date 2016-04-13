#ifndef CORE_PRIMITIVES_STRINGS_STRING_HPP_
#define CORE_PRIMITIVES_STRINGS_STRING_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::string
// Description
//   A class for handling character strings derived from std::string.
//   Strings may contain any characters and therefore are delimited by quotes
//   for IO : "any list of characters".
//   Used as a base class for word and fileName.
// See Also
//   mousse::findEtcFile() for information about the site/user OpenFOAM
//   configuration directory
// SourceFiles
//   string.cpp
//   string_io.cpp

#include "char.hpp"
#include "hasher.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>


namespace mousse {

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators
class string;
Istream& operator>>(Istream&, string&);
Ostream& operator<<(Ostream&, const string&);
Ostream& operator<<(Ostream&, const std::string&);


class string
:
  public std::string
{
public:
  // Static data members
    static const char* const typeName;
    static int debug;
    //- An empty string
    static const string null;
  //- Hashing function class, shared by all the derived classes
  class hash
  {
  public:
    hash()
    {}
    inline unsigned operator()(const string&, unsigned seed = 0) const;
  };
  // Constructors
    //- Construct null
    inline string();
    //- Construct from std::string
    inline string(const std::string&);
    //- Construct as copy of character array
    inline string(const char*);
    //- Construct as copy of specified number of characters
    inline string(const char*, const size_type);
    //- Construct from a single character
    inline string(const char);
    //- Construct from Istream
    string(Istream&);
  // Member Functions
    //- Count and return the number of a given character in the string
    size_type count(const char) const;
    //- Is this string type valid?
    template<class String>
    static inline bool valid(const string&);
    //- Does this string have particular meta-characters?
    //  The meta characters can be optionally quoted.
    template<class String>
    static inline bool meta(const string&, const char quote='\\');
    //- Strip invalid characters from the given string
    template<class String>
    static inline bool stripInvalid(string&);
    //- Return a valid String from the given string
    template<class String>
    static inline String validate(const string&);
    //- Return a String with quoted meta-characters from the given string
    template<class String>
    static inline string quotemeta(const string&, const char quote='\\');
    //- True when strings match literally
    inline bool match(const std::string&) const;
    //- Avoid masking the normal std::string replace
    using std::string::replace;
    //- Replace first occurence of sub-string oldStr with newStr
    //  starting at start
    string& replace
    (
      const string& oldStr,
      const string& newStr,
      size_type start = 0
    );
    //- Replace all occurences of sub-string oldStr with newStr
    //  starting at start
    string& replaceAll
    (
      const string& oldStr,
      const string& newStr,
      size_type start = 0
    );
    //- Expand initial tildes and all occurences of environment variables
    //  Expansion includes:
    //  -# environment variables
    //    - "$VAR", "${VAR}"
    //  -# current directory
    //    - leading "./" : the current directory
    //  -# tilde expansion
    //    - leading "~/" : home directory
    //    - leading "~user" : home directory for specified user
    //    - leading "~OpenFOAM" : site/user OpenFOAM configuration directory
    //
    //  Any unknown entries are removed silently if allowEmpty is true
    //  \sa
    //  mousse::findEtcFile
    string& expand(const bool allowEmpty = false);
    //- Remove repeated characters returning true if string changed
    bool removeRepeated(const char);
    //- Return string with repeated characters removed
    string removeRepeated(const char) const;
    //- Remove trailing character returning true if string changed
    bool removeTrailing(const char);
    //- Return string with trailing character removed
    string removeTrailing(const char) const;
  // Member Operators
    //- Return the sub-string from the i-th character for \a n characters
    inline string operator()
    (
      const size_type i,
      const size_type n
    ) const;
    //- Return the sub-string from the first character for \a n characters
    inline string operator()
    (
      const size_type n
    ) const;
  // IOstream Operators
    friend Istream& operator>>(Istream&, string&);
    friend Ostream& operator<<(Ostream&, const string&);
};
}  // namespace mousse


// Constructors 
inline mousse::string::string()
{}


inline mousse::string::string(const std::string& str)
:
  std::string{str}
{}


// Copy character array
inline mousse::string::string(const char* str)
:
  std::string{str}
{}


// Construct from a given number of characters in a character array
inline mousse::string::string(const char* str, const size_type len)
:
  std::string{str, len}
{}


// Construct from a single character
inline mousse::string::string(const char c)
:
  std::string{1, c}
{}


// Member Functions 
template<class String>
inline bool mousse::string::valid(const string& str)
{
  for (const_iterator iter = str.begin(); iter != str.end(); ++iter) {
    if (!String::valid(*iter)) {
      return false;
    }
  }
  return true;
}


template<class String>
inline bool mousse::string::stripInvalid(string& str)
{
  if (!valid<String>(str)) {
    size_type nValid = 0;
    iterator iter2 = str.begin();
    for (const_iterator iter1 = iter2;
         iter1 != const_cast<const string&>(str).end();
         iter1++) {
      char c = *iter1;
      if (String::valid(c)) {
        *iter2 = c;
        ++iter2;
        ++nValid;
      }
    }
    str.resize(nValid);
    return true;
  }
  return false;
}


template<class String>
inline bool mousse::string::meta(const string& str, const char quote)
{
  int escaped = 0;
  for (const_iterator iter = str.begin(); iter != str.end(); ++iter) {
    if (quote && *iter == quote) {
      escaped ^= 1;  // toggle state
    } else if (escaped) {
      escaped = false;
    } else if (String::meta(*iter)) {
      return true;
    }
  }
  return false;
}


template<class String>
inline mousse::string
mousse::string::quotemeta(const string& str, const char quote)
{
  if (!quote) {
    return str;
  }
  string sQuoted;
  sQuoted.reserve(2*str.length());
  int escaped = 0;
  for (const_iterator iter = str.begin(); iter != str.end(); ++iter) {
    if (*iter == quote) {
      escaped ^= 1;  // toggle state
    } else if (escaped) {
      escaped = 0;
    } else if (String::meta(*iter)) {
      sQuoted += quote;
    }
    sQuoted += *iter;
  }
  sQuoted.resize(sQuoted.length());
  return sQuoted;
}


template<class String>
inline String mousse::string::validate(const string& str)
{
  string ss = str;
  stripInvalid<String>(ss);
  return ss;
}


inline bool mousse::string::match(const std::string& str) const
{
  // check as string
  return (str == *this);
}


// Member Operators 
inline mousse::string mousse::string::operator()
(
  const size_type i,
  const size_type n
) const
{
  return substr(i, n);
}


inline mousse::string mousse::string::operator()(const size_type n) const
{
  return substr(0, n);
}


inline unsigned mousse::string::hash::operator()
(
  const string& key,
  unsigned seed
) const
{
  return Hasher(key.data(), key.size(), seed);
}

#endif
