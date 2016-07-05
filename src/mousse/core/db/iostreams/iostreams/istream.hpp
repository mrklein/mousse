#ifndef CORE_DB_IOSTREAMS_IOSTREAMS_ISTREAM_HPP_
#define CORE_DB_IOSTREAMS_IOSTREAMS_ISTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::Istream
// Description
//   An Istream is an abstract base class for all input systems
//   (streams, files, token lists etc).  The basic operations
//   are construct, close, read token, read primitive and read binary
//   block.
//   In addition, version control and line number counting is incorporated.
//   Usually one would use the read primitive member functions, but if one
//   were reading a stream on unknown data sequence one can read token by
//   token, and then analyse.

#include "iostream.hpp"
#include "token.hpp"


namespace mousse {

class Istream
:
  public IOstream
{
  // Private data
    //- Has a token been put back on the stream?
    bool putBack_;
    //- The last token put back on the stream
    token putBackToken_;
public:
  // Constructors
    //- Set stream status
    Istream
    (
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    )
    :
      IOstream{format, version, compression},
      putBack_{false}
    {}
  //- Destructor
  virtual ~Istream()
  {}
  // Member functions
    // Read functions
      //- Put back token
      //  Only a single put back is permitted
      void putBack(const token&);
      //- Get the put back token if there is one and return true.
      //  Return false if no put back token is available.
      bool getBack(token&);
      //- Peek at the put back token without removing it.
      //  Returns false if no put back token is available and set the
      //  token to undefined.
      bool peekBack(token&);
      //- Return next token from stream
      virtual Istream& read(token&) = 0;
      //- Read a character
      virtual Istream& read(char&) = 0;
      //- Read a word
      virtual Istream& read(word&) = 0;
      // Read a string (including enclosing double-quotes)
      virtual Istream& read(string&) = 0;
      //- Read a label
      virtual Istream& read(label&) = 0;
      //- Read a floatScalar
      virtual Istream& read(floatScalar&) = 0;
      //- Read a doubleScalar
      virtual Istream& read(doubleScalar&) = 0;
      //- Read binary block
      virtual Istream& read(char*, std::streamsize) = 0;
      //- Rewind and return the stream so that it may be read again
      virtual Istream& rewind() = 0;
    // Read List punctuation tokens
      Istream& readBegin(const char* funcName);
      Istream& readEnd(const char* funcName);
      Istream& readEndBegin(const char* funcName);
      char readBeginList(const char* funcName);
      char readEndList(const char* funcName);
  // Member operators
    //- Return a non-const reference to const Istream
    //  Needed for read-constructors where the stream argument is temporary:
    //  e.g. thing thisThing(IFstream("thingFileName")());
    Istream& operator()() const;
};


// Manipulators (not taking arguments)
typedef Istream& (*IstreamManip)(Istream&);

//- operator>> handling for manipulators without arguments
inline Istream& operator>>(Istream& is, IstreamManip f)
{
  return f(is);
}

//- operator>> handling for manipulators without arguments
inline Istream& operator>>(Istream& is, IOstreamManip f)
{
  f(is);
  return is;
}

}  // namespace mousse

#include "hash_table.ipp"

#endif
