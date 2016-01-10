// mousse: CFD toolbox
// Copyright (C) 2011-2014 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::fileName
// Description
//   A class for handling file names.
//   A fileName is a string of characters without whitespace or quotes.
//   A fileName can be
//    - constructed from a char*, a string or a word
//    - concatenated by adding a '/' separator
//    - decomposed into the path, name or component list
//    - interrogated for type and access mode
//   The string::expand() method expands environment variables, etc,
// SourceFiles
//   file_name.cpp
//   file_name_io.cpp

#ifndef file_name_hpp_
#define file_name_hpp_

#include "word.hpp"

namespace mousse
{
template<class T> class List;
typedef List<word> wordList;
// Forward declaration of friend functions and operators
class fileName;
Istream& operator>>(Istream&, fileName&);
Ostream& operator<<(Ostream&, const fileName&);
class fileName
:
  public string
{
  // Private Member Functions
    //- Strip invalid characters
    inline void stripInvalid();
public:
  //- Enumerations to handle file types and modes.
  enum Type
  {
    UNDEFINED,
    FILE,
    DIRECTORY,
    LINK
  };
  // Static data members
    static const char* const typeName;
    static int debug;
    //- An empty fileName
    static const fileName null;
  // Constructors
    //- Construct null
    inline fileName();
    //- Construct as copy
    inline fileName(const fileName&);
    //- Construct as copy of word
    inline fileName(const word&);
    //- Construct as copy of string
    inline fileName(const string&);
    //- Construct as copy of std::string
    inline fileName(const std::string&);
    //- Construct as copy of character array
    inline fileName(const char*);
    //- Construct by concatenating elements of wordList separated by '/'
    explicit fileName(const wordList&);
    //- Construct from Istream
    fileName(Istream&);
  // Member functions
    //- Is this character valid for a fileName?
    inline static bool valid(char);
    //- Cleanup file name
    //  eg, remove repeated slashes, etc.
    bool clean();
    //- Cleanup file name
    //  eg, remove repeated slashes, etc.
    fileName clean() const;
    // Interrogation
      //- Return the file type: FILE, DIRECTORY or UNDEFINED
      Type type() const;
      //- Return true if file name is absolute
      bool isAbsolute() const;
      //- Convert from relative to absolute
      fileName& toAbsolute();
    // Decomposition
      //- Return file name (part beyond last /)
      word name() const;
      //- Return file name (part beyond last /), subsitute for MOUSSE_CASE
      string caseName() const;
      //- Return file name, optionally without extension
      word name(const bool noExt) const;
      //- Return directory path name (part before last /)
      fileName path() const;
      //- Return file name without extension (part before last .)
      fileName lessExt() const;
      //- Return file name extension (part after last .)
      word ext() const;
      //- Return path components as wordList
      wordList components(const char delimiter='/') const;
      //- Return a single component of the path
      word component(const size_type, const char delimiter='/') const;
  // Member operators
    // Assignment
      const fileName& operator=(const fileName&);
      const fileName& operator=(const word&);
      const fileName& operator=(const string&);
      const fileName& operator=(const std::string&);
      const fileName& operator=(const char*);
  // IOstream operators
    friend Istream& operator>>(Istream&, fileName&);
    friend Ostream& operator<<(Ostream&, const fileName&);
};
//- Assemble words and fileNames as pathnames by adding a '/' separator
fileName operator/(const string&, const string&);
}  // namespace mousse

// Private Member Functions 
inline void mousse::fileName::stripInvalid()
{
  if (debug && string::stripInvalid<fileName>(*this))
  {
    std::cerr
      << "fileName::stripInvalid() called for invalid fileName "
      << this->c_str() << std::endl;
    if (debug > 1)
    {
      std::cerr
        << "    For debug level (= " << debug
        << ") > 1 this is considered fatal" << std::endl;
      std::abort();
    }
    removeRepeated('/');
    removeTrailing('/');
  }
}
// Constructors 
inline mousse::fileName::fileName()
:
  string()
{}
inline mousse::fileName::fileName(const fileName& fn)
:
  string(fn)
{}
inline mousse::fileName::fileName(const word& w)
:
  string(w)
{}
inline mousse::fileName::fileName(const string& str)
:
  string(str)
{
  stripInvalid();
}
inline mousse::fileName::fileName(const std::string& str)
:
  string(str)
{
  stripInvalid();
}
inline mousse::fileName::fileName(const char* str)
:
  string(str)
{
  stripInvalid();
}
// Member Functions 
inline bool mousse::fileName::valid(char c)
{
  return
  (
    !isspace(c)
  && c != '"'   // string quote
  && c != '\''  // string quote
  );
}

// #include "file_name_i.hpp"

#endif
