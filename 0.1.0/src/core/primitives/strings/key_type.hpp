// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::keyType
// Description
//   A class for handling keywords in dictionaries.
//   A keyType is the keyword of a dictionary.
//   It differs from word in that it accepts patterns (regular expressions).
// SourceFiles
//   key_type.cpp
#ifndef key_type_hpp_
#define key_type_hpp_
#include "word.hpp"
namespace mousse
{
// Forward declaration of classes
class Istream;
class Ostream;
class keyType
:
  public word
{
  // Private data
    //- Is the keyType a pattern (regular expression)
    bool isPattern_;
  // Private Member Functions
    //- Disallow assignments where we cannot determine string/word type
    void operator=(const std::string&);
public:
  // Static data members
    //- An empty keyType
    static const keyType null;
  // Constructors
    //- Construct null
    inline keyType();
    //- Construct as copy
    inline keyType(const keyType&);
    //- Construct as copy of word. Not treated as a regular expression
    inline keyType(const word&);
    //- Construct as copy of string. Treat as regular expression.
    inline keyType(const string&);
    //- Construct as copy of character array.
    //  Not treated as a regular expression
    inline keyType(const char*);
    //- Construct as copy of std::string with specified treatment
    inline keyType(const std::string&, const bool isPattern);
    //- Construct from Istream
    //  Treat as regular expression if surrounded by quotation marks.
    keyType(Istream&);
  // Member functions
    //- Should be treated as a match rather than a literal string
    inline bool isPattern() const;
    //- Smart match as regular expression or as a string
    //  Optionally force a literal match only
    bool match(const std::string&, bool literalMatch=false) const;
  // Member operators
    // Assignment
      //- Assignment operator
      inline const keyType& operator=(const keyType&);
      //- Assign as word, not as non regular expression
      inline const keyType& operator=(const word&);
      //- Assign as regular expression
      inline const keyType& operator=(const string&);
      //- Assign as word, not as non regular expression
      inline const keyType& operator=(const char*);
  // IOstream operators
    friend Istream& operator>>(Istream&, keyType&);
    friend Ostream& operator<<(Ostream&, const keyType&);
};
}  // namespace mousse
#include "key_type_i.hpp"
#endif
