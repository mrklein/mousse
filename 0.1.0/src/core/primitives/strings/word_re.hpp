// mousse: CFD toolbox
// Copyright (C) 2011-2013 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::wordRe
// Description
//   A wordRe is a word, but can also have a regular expression for matching
//   words.
//   By default the constructors will generally preserve the argument as a
//   string literal and the assignment operators will use the wordRe::DETECT
//   compOption to scan the string for regular expression meta characters
//   and/or invalid word characters and react accordingly.
//   The exceptions are when constructing/assigning from another
//   mousse::wordRe (preserve the same type) or from a mousse::word (always
//   literal).
// Note
//   If the string contents are changed - eg, by the operator+=() or by
//   string::replace(), etc - it will be necessary to use compile() or
//   recompile() to synchronize the regular expression.
// SourceFiles
//   word_re.cpp
#ifndef word_re_hpp_
#define word_re_hpp_
#include "word.hpp"
#include "reg_exp.hpp"
#include "key_type.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class wordRe;
class Istream;
class Ostream;
Istream& operator>>(Istream&, wordRe&);
Ostream& operator<<(Ostream&, const wordRe&);
class wordRe
:
  public word
{
  // Private member data
    //- The regular expression
    mutable regExp re_;
public:
  // Static data members
    //- An empty wordRe
    static const wordRe null;
  // Public data types
    //- Enumeration with compile options
    //  Note that 'REGEXP' is implicit if 'NOCASE' is specified alone.
    enum compOption
    {
      LITERAL = 0, /*!< treat as a string literal */
      DETECT  = 1, /*!< treat as regular expression */
      REGEXP  = 2, /*!< detect if the string contains meta-characters */
      NOCASE  = 4, /*!< ignore case in regular expression */
      DETECT_NOCASE = DETECT | NOCASE,
      REGEXP_NOCASE = REGEXP | NOCASE
    };
    //- Is this a meta character?
    static inline bool meta(char);
    //- Test string for regular expression meta characters
    static inline bool isPattern(const string&);
  // Constructors
    //- Construct null
    inline wordRe();
    //- Construct as copy
    inline wordRe(const wordRe&);
    //- Construct from keyType
    inline explicit wordRe(const keyType&);
    //- Construct from keyType
    inline wordRe(const keyType&, const compOption);
    //- Construct as copy of word
    inline explicit wordRe(const word&);
    //- Construct as copy of character array
    //  Optionally specify how it should be treated.
    inline explicit wordRe(const char*, const compOption = LITERAL);
    //- Construct as copy of string.
    //  Optionally specify how it should be treated.
    inline explicit wordRe(const string&, const compOption = LITERAL);
    //- Construct as copy of std::string
    //  Optionally specify how it should be treated.
    inline explicit wordRe(const std::string&, const compOption = LITERAL);
    //- Construct from Istream
    //  Words are treated as literals, strings with an auto-test
    wordRe(Istream&);
  // Member functions
    // Access
      //- Should be treated as a match rather than a literal string?
      inline bool isPattern() const;
    // Infrastructure
      //- Compile the regular expression
      inline bool compile() const;
      //- Possibly compile the regular expression, with greater control
      inline bool compile(const compOption) const;
      //- Recompile an existing regular expression
      inline bool recompile() const;
      //- Frees precompiled regular expression, making wordRe a literal.
      //  Optionally strips invalid word characters
      inline void uncompile(const bool doStripInvalid = false) const;
    // Editing
      //- Copy string, auto-test for regular expression or other options
      inline void set(const std::string&, const compOption = DETECT);
      //- Copy string, auto-test for regular expression or other options
      inline void set(const char*, const compOption = DETECT);
      //- Clear string and precompiled regular expression
      inline void clear();
    // Searching
      //- Smart match as regular expression or as a string
      //  Optionally force a literal match only
      inline bool match
      (
        const std::string&,
        bool literalMatch = false
      ) const;
    // Miscellaneous
      //- Return a string with quoted meta-characters
      inline string quotemeta() const;
      //- Output some basic info
      Ostream& info(Ostream&) const;
  // Member operators
    // Assignment
      //- Assign copy
      //  Always case sensitive
      inline const wordRe& operator=(const wordRe&);
      //- Copy word, never a regular expression
      inline const wordRe& operator=(const word&);
      //- Copy keyType, auto-test for regular expression
      //  Always case sensitive
      inline const wordRe& operator=(const keyType&);
      //- Copy string, auto-test for regular expression
      //  Always case sensitive
      inline const wordRe& operator=(const string&);
      //- Copy string, auto-test for regular expression
      //  Always case sensitive
      inline const wordRe& operator=(const std::string&);
      //- Copy string, auto-test for regular expression
      //  Always case sensitive
      inline const wordRe& operator=(const char*);
  // IOstream operators
    friend Istream& operator>>(Istream&, wordRe&);
    friend Ostream& operator<<(Ostream&, const wordRe&);
};
}  // namespace mousse

// Static Member Functions
inline bool mousse::wordRe::meta(char c)
{
  return regExp::meta(c);
}
inline bool mousse::wordRe::isPattern(const string& str)
{
  return string::meta<regExp>(str);
}
// Constructors 
inline mousse::wordRe::wordRe()
:
  word{},
  re_{}
{}
inline mousse::wordRe::wordRe(const wordRe& str)
:
  word{str},
  re_{}
{
  if (str.isPattern())
  {
    compile();
  }
}
inline mousse::wordRe::wordRe(const word& str)
:
  word{str},
  re_{}
{}
inline mousse::wordRe::wordRe(const keyType& str)
:
  word{str, false},
  re_{}
{
  if (str.isPattern())
  {
    compile();
  }
}
inline mousse::wordRe::wordRe(const keyType& str, const compOption opt)
:
  word{str, false},
  re_{}
{
  if (str.isPattern())
  {
    compile(opt);
  }
}
inline mousse::wordRe::wordRe(const char* str, const compOption opt)
:
  word{str, false},
  re_{}
{
  compile(opt);
}
inline mousse::wordRe::wordRe(const string& str, const compOption opt)
:
  word{str, false},
  re_{}
{
  compile(opt);
}
inline mousse::wordRe::wordRe(const std::string& str, const compOption opt)
:
  word{str, false},
  re_{}
{
  compile(opt);
}
// Member Functions 
inline bool mousse::wordRe::isPattern() const
{
  return re_.exists();
}
inline bool mousse::wordRe::compile(const compOption opt) const
{
  bool doCompile = false;
  if (opt & wordRe::REGEXP)
  {
    doCompile = true;
  }
  else if (opt & wordRe::DETECT)
  {
    if (string::meta<regExp>(*this) || !string::valid<word>(*this))
    {
      doCompile = true;
    }
  }
  else if (opt & wordRe::NOCASE)
  {
    doCompile = true;
  }
  if (doCompile)
  {
    re_.set(*this, (opt & wordRe::NOCASE));
  }
  else
  {
    re_.clear();
  }
  return re_.exists();
}
inline bool mousse::wordRe::compile() const
{
  re_ = *this;
  return re_.exists();
}
inline bool mousse::wordRe::recompile() const
{
  if (re_.exists())
  {
    re_ = *this;
  }
  return re_.exists();
}
inline void mousse::wordRe::uncompile(const bool doStripInvalid) const
{
  if (re_.clear())
  {
    // skip stripping unless debug is active to avoid costly operations
    if (word::debug && doStripInvalid)
    {
      string::stripInvalid<word>
      (
        const_cast<word&>(static_cast<const word&>(*this))
      );
    }
  }
}
inline void mousse::wordRe::clear()
{
  word::clear();
  re_.clear();
}
inline bool mousse::wordRe::match(const std::string& str, bool literalMatch) const
{
  if (literalMatch || !re_.exists())
  {
    // check as string
    return (str == *this);
  }
  else
  {
    // check as regex
    return re_.match(str);
  }
}
inline mousse::string mousse::wordRe::quotemeta() const
{
  return string::quotemeta<regExp>(*this);
}
inline void mousse::wordRe::set(const std::string& str, const compOption opt)
{
  string::operator=(str);
  compile(opt);
}
inline void mousse::wordRe::set(const char* str, const compOption opt)
{
  string::operator=(str);
  compile(opt);
}
// Member Operators 
inline const mousse::wordRe& mousse::wordRe::operator=(const wordRe& str)
{
  string::operator=(str);
  if (str.isPattern())
  {
    compile();
  }
  else
  {
    re_.clear();
  }
  return *this;
}
inline const mousse::wordRe& mousse::wordRe::operator=(const word& str)
{
  word::operator=(str);
  re_.clear();
  return *this;
}
inline const mousse::wordRe& mousse::wordRe::operator=(const keyType& str)
{
  string::operator=(str);
  if (str.isPattern())
  {
    compile();
  }
  return *this;
}
inline const mousse::wordRe& mousse::wordRe::operator=(const string& str)
{
  string::operator=(str);
  compile(DETECT);  // auto-detect regex
  return *this;
}
inline const mousse::wordRe& mousse::wordRe::operator=(const std::string& str)
{
  string::operator=(str);
  compile(DETECT);  // auto-detect regex
  return *this;
}
inline const mousse::wordRe& mousse::wordRe::operator=(const char* str)
{
  string::operator=(str);
  compile(DETECT);  // auto-detect regex
  return *this;
}
#endif
