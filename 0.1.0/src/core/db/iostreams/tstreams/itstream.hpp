#ifndef CORE_DB_IOSTREAMS_TSTREAMS_ITSTREAM_HPP_
#define CORE_DB_IOSTREAMS_TSTREAMS_ITSTREAM_HPP_

// mousse: CFD toolbox
// Copyright (C) 2011 OpenFOAM Foundation
// Copyright (C) 2016 mousse project
// Class
//   mousse::ITstream
// Description
//   Input token stream.
// SourceFiles
//   itstream.cpp


#include "istream.hpp"
#include "token_list.hpp"

namespace mousse
{
class ITstream
:
  public Istream,
  public tokenList
{
  // Private data
    //- Name of ITstream
    fileName name_;
    //- Index of token currently being read
    label tokenIndex_;
public:
  // Constructors
    //- Construct from components
    ITstream
    (
      const string& name,
      const UList<token>& tokens,
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      Istream{format, version},
      tokenList{tokens},
      name_{name},
      tokenIndex_{0}
    {
      setOpened();
      setGood();
    }
    //- Construct from components, transferring the tokens
    ITstream
    (
      const string& name,
      const Xfer<List<token> >& tokens,
      streamFormat format=ASCII,
      versionNumber version=currentVersion
    )
    :
      Istream{format, version},
      tokenList{tokens},
      name_{name},
      tokenIndex_{0}
    {
      setOpened();
      setGood();
    }
    //- Construct as copy
    ITstream(const ITstream& its)
    :
      Istream{ASCII, currentVersion},
      tokenList{its},
      name_{its.name_},
      tokenIndex_{0}
    {
      setOpened();
      setGood();
    }
    //- Destructor
    virtual ~ITstream()
    {}
  // Member functions
    // Inquiry
      //- Return the name of the stream
      const fileName& name() const
      {
        return name_;
      }
      //- Return non-const access to the name of the stream
      fileName& name()
      {
        return name_;
      }
      //- Return the current token index
      label tokenIndex() const
      {
        return tokenIndex_;
      }
      //- Return non-const access to the current token index
      label& tokenIndex()
      {
        return tokenIndex_;
      }
      //- Return the number of remaining tokens
      label nRemainingTokens() const
      {
        return size() - tokenIndex_;
      }
      //- Return flags of output stream
      ios_base::fmtflags flags() const
      {
        return ios_base::fmtflags(0);
      }
    // Read functions
      //- Return next token from stream
      virtual Istream& read(token&);
      //- Read a character
      virtual Istream& read(char&);
      //- Read a word
      virtual Istream& read(word&);
      // Read a string (including enclosing double-quotes)
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
    // Edit
      //- Set flags of stream
      ios_base::fmtflags flags(const ios_base::fmtflags)
      {
        return ios_base::fmtflags(0);
      }
    // Print
      //- Print description of IOstream to Ostream
      void print(Ostream&) const;
};
}  // namespace mousse
#endif
