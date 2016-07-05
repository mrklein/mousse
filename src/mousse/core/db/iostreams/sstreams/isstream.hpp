#ifndef CORE_DB_IOSTREAMS_SSTREAMS_ISSTREAM_HPP_
#define CORE_DB_IOSTREAMS_SSTREAMS_ISSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011-2012 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ISstream
// Description
//   Generic input stream.

#include "istream.hpp"
#include "file_name.hpp"
#include <iostream>


namespace mousse {

class ISstream
:
  public Istream
{

  // Private data
    fileName name_;
    istream& is_;

  // Private Member Functions
    char nextValid();
    void readWordToken(token&);

  // Private Member Functions

    //- Read a verbatim string (excluding block delimiters).
    Istream& readVerbatim(string&);

    //- Read a variable name (includes '{')
    Istream& readVariable(string&);

public:

  // Constructors

    //- Construct as wrapper around istream
    inline ISstream
    (
      istream& is,
      const string& name,
      streamFormat format=ASCII,
      versionNumber version=currentVersion,
      compressionType compression=UNCOMPRESSED
    );

    //- Disallow default bitwise assignment
    ISstream& operator=(const ISstream&) = delete;

  //- Destructor
  virtual ~ISstream()
  {}

  // Member functions

    // Inquiry

      //- Return the name of the stream
      //  Useful for Fstream to return the filename
      virtual const fileName& name() const
      {
        return name_;
      }

      //- Return non-const access to the name of the stream
      //  Useful to alter the stream name
      virtual fileName& name()
      {
        return name_;
      }

      //- Return flags of output stream
      virtual ios_base::fmtflags flags() const;

    // Read functions

      //- Raw, low-level get character function.
      inline ISstream& get(char&);

      //- Raw, low-level peek function.
      //  Does not remove the character from the stream.
      //  Returns the next character in the stream or EOF if the
      //  end of file is read.
      inline int peek();

      //- Raw, low-level getline into a string function.
      inline ISstream& getLine(string&);

      //- Raw, low-level putback character function.
      inline ISstream& putback(const char&);

      //- Return next token from stream
      virtual Istream& read(token&);

      //- Read a character
      virtual Istream& read(char&);

      //- Read a word
      virtual Istream& read(word&);

      //- Read a string (including enclosing double-quotes).
      //  Backslashes are retained, except when escaping double-quotes
      //  and an embedded newline character.
      virtual Istream& read(string&);

      //- Read a label
      virtual Istream& read(label&);

      //- Read a floatScalar
      virtual Istream& read(floatScalar&);

      //- Read a doubleScalar
      virtual Istream& read(doubleScalar&);

      //- Read binary block
      virtual Istream& read(char*, std::streamsize);

      //- Rewind and return the stream so that it may be read again
      virtual Istream& rewind();

    // Stream state functions

      //- Set flags of output stream
      virtual ios_base::fmtflags flags(const ios_base::fmtflags flags);

    // STL stream

      //- Access to underlying std::istream
      virtual istream& stdStream()
      {
        return is_;
      }

      //- Const access to underlying std::istream
      virtual const istream& stdStream() const
      {
        return is_;
      }

    // Print

      //- Print description of IOstream to Ostream
      virtual void print(Ostream&) const;

};

}  // namespace mousse

// Constructors 
inline mousse::ISstream::ISstream
(
  istream& is,
  const string& name,
  streamFormat format,
  versionNumber version,
  compressionType compression
)
:
  Istream{format, version, compression},
  name_{name},
  is_{is}
{
  if (is_.good()) {
    setOpened();
    setGood();
  } else {
    setState(is_.rdstate());
  }
}


// Member Functions 
inline mousse::ISstream& mousse::ISstream::get(char& c)
{
  is_.get(c);
  setState(is_.rdstate());
  if (c == '\n') {
    lineNumber_++;
  }
  return *this;
}

inline int mousse::ISstream::peek()
{
  return is_.peek();
}

inline mousse::ISstream& mousse::ISstream::getLine(string& s)
{
  getline(is_, s);
  setState(is_.rdstate());
  lineNumber_++;
  return *this;
}

inline mousse::ISstream& mousse::ISstream::putback(const char& c)
{
  if (c == '\n') {
    lineNumber_--;
  }
  if (!is_.putback(c)) {
    setBad();
  }
  setState(is_.rdstate());
  return *this;
}

#endif
