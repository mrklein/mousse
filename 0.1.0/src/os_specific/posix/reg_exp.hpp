// mousse: CFD toolbox
// Copyright (C) 2011-2015 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::regExp
// Description
//   Wrapper around POSIX extended regular expressions.
//   The PCRE '(?i)' extension is provided to compile the regular expression
//   as being case-insensitive.
// SeeAlso
//   The manpage regex(7) for more information about POSIX regular expressions.
//   These differ somewhat from \c Perl and \c sed regular expressions.
// SourceFiles
//   reg_exp.cpp
#ifndef reg_exp_hpp_
#define reg_exp_hpp_
#include <regex.h>
#include <string>
namespace mousse
{
// Forward declaration of classes
class string;
template<class T> class List;
class regExp
{
  // Private data
    //- Precompiled regular expression
    mutable regex_t* preg_;
  // Private Member Functions
    //- Return true if it matches and sets the sub-groups matched.
    //  Templated to support both std::string and mousse::string
    template<class StringType>
    bool matchGrouping
    (
      const std::string&,
      List<StringType>& groups
    ) const;
public:
  // Static Member Functions
    //- Is character a regular expression meta-character?
    //  any character: '.' \n
    //  quantifiers: '*', '+', '?' \n
    //  grouping: '(', '|', ')' \n
    //  range: '[', ']' \n
    //
    //  Don't bother checking for '{digit}' bounds
    inline static bool meta(char c)
    {
      return
      (
        (c == '.')                           // any character
      || (c == '*' || c == '+' || c == '?')   // quantifiers
      || (c == '(' || c == ')' || c == '|')   // grouping/branching
      || (c == '[' || c == ']')               // range
      );
    }
  // Constructors
    //- Construct null
    regExp();
    //- Construct from character array, optionally ignoring case
    regExp(const char*, const bool ignoreCase=false);
    //- Construct from std::string (or string), optionally ignoring case
    regExp(const std::string&, const bool ignoreCase=false);
    //- Disallow default bitwise copy construct
    regExp(const regExp&) = delete;
    //- Disallow default bitwise assignment
    regExp& operator=(const regExp&) = delete;
  //- Destructor
  ~regExp();
  // Member functions
    // Access
      //- Return true if a precompiled expression does not exist
      inline bool empty() const
      {
        return !preg_;
      }
      //- Does a precompiled expression exist?
      inline bool exists() const
      {
        return preg_ ? true : false;
      }
      //- Return the number of (groups)
      inline int ngroups() const
      {
        return preg_ ? preg_->re_nsub : 0;
      }
    // Editing
      //- Compile pattern into a regular expression,
      //  optionally ignoring case
      void set(const char*, const bool ignoreCase=false) const;
      //- Compile pattern into a regular expression,
      //  optionally ignoring case
      void set(const std::string&, const bool ignoreCase=false) const;
      //- Release precompiled expression.
      //  Returns true if precompiled expression existed before clear
      bool clear() const;
    // Searching
      //- Find position within string.
      //  Returns the index where it begins or string::npos if not found
      std::string::size_type find(const std::string& str) const;
      //- Return true if it matches the entire string
      //  The begin-of-line (^) and end-of-line ($) anchors are implicit
      bool match(const std::string&) const;
      //- Return true if it matches and sets the sub-groups matched
      //  The begin-of-line (^) and end-of-line ($) anchors are implicit
      bool match(const std::string&, List<std::string>& groups) const;
      //- Return true if it matches and sets the sub-groups matched
      //  The begin-of-line (^) and end-of-line ($) anchors are implicit
      bool match(const std::string&, List<string>& groups) const;
      //- Return true if the regex was found within string
      bool search(const std::string& str) const
      {
        return std::string::npos != find(str);
      }
  // Member Operators
    //- Assign and compile pattern from a character array
    //  Always case sensitive
    void operator=(const char*);
    //- Assign and compile pattern from string
    //  Always case sensitive
    void operator=(const std::string&);
};
}  // namespace mousse
#endif
