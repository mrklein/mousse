// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::word
// Description
//   A class for handling words, derived from string.
//   A word is a string of characters without whitespace, quotes, slashes,
//   semicolons or brace brackets. Words are delimited by whitespace.
// SourceFiles
//   word.cpp
//   word_io.cpp
#ifndef word_hpp_
#define word_hpp_
#include "string.hpp"
namespace mousse
{
// Forward declaration of friend functions and operators
class word;
inline word operator&(const word&, const word&);
Istream& operator>>(Istream&, word&);
Ostream& operator<<(Ostream&, const word&);
class word
:
  public string
{
  // Private Member Functions
    //- Strip invalid characters from this word
    inline void stripInvalid();
public:
  // Static data members
    static const char* const typeName;
    static int debug;
    //- An empty word
    static const word null;
  // Constructors
    //- Construct null
    inline word();
    //- Construct as copy
    inline word(const word&);
    //- Construct as copy of character array
    inline word(const char*, const bool doStripInvalid=true);
    //- Construct as copy with a maximum number of characters
    inline word
    (
      const char*,
      const size_type,
      const bool doStripInvalid
    );
    //- Construct as copy of string
    inline word(const string&, const bool doStripInvalid=true);
    //- Construct as copy of std::string
    inline word(const std::string&, const bool doStripInvalid=true);
    //- Construct from Istream
    word(Istream&);
  // Member functions
    //- Is this character valid for a word
    inline static bool valid(char);
  // Member operators
    // Assignment
      inline const word& operator=(const word&);
      inline const word& operator=(const string&);
      inline const word& operator=(const std::string&);
      inline const word& operator=(const char*);
  // Friend Operators
    friend word operator&(const word&, const word&);
  // IOstream operators
    friend Istream& operator>>(Istream&, word&);
    friend Ostream& operator<<(Ostream&, const word&);
};
}  // namespace mousse
#include "word_i.hpp"
#endif
